#include <QFile>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QTimer>
#include <qjson/parser.h>
#include "dropboxrequest.h"
#include "dropbox.h"

#include <QDebug>

#define BUF_SIZE 512

QCloud::Request::Error DropboxRequest::error()
{
    return m_error;
}

void DropboxRequest::sendRequest (const QUrl& url, const QOAuth::HttpMethod& method, QIODevice* device, QOAuth::ParamMap paramMap)
{
    m_error = NoError;
    m_reply = 0;
    QNetworkRequest request (url);
    if (method == QOAuth::POST) {
        for (QOAuth::ParamMap::iterator it = paramMap.begin(); it != paramMap.end(); it++) {
            it.value() = QUrl::toPercentEncoding(QString::fromUtf8(it.value()));
        }
        qDebug() << paramMap;
        request.setRawHeader ("Authorization", m_dropbox->authorizationString (url, method, paramMap));
        qDebug() << m_dropbox->authorizationString (url, method, paramMap, QOAuth::ParseForRequestContent);
        m_reply = m_dropbox->networkAccessManager()->post (request, m_dropbox->inlineString (paramMap, QOAuth::ParseForRequestContent));
    } else {
        request.setRawHeader ("Authorization", m_dropbox->authorizationString (url, method));
        if (method == QOAuth::GET)
            m_reply = m_dropbox->networkAccessManager()->get (request);
        else if (method == QOAuth::PUT)
            m_reply = m_dropbox->networkAccessManager()->put (request, device);
        else {
            m_error = Request::NetworkError;
            qDebug() << "Not supported method";
            return ;
        }
    }
    connect (m_reply, SIGNAL (readyRead()) , this , SLOT (readyForRead()));
    connect (m_reply, SIGNAL (finished()) , this , SLOT (replyFinished()));
}

QString DropboxRequest::getRootType()
{
    if (m_dropbox->m_globalAccess)
        return "dropbox";
    else
        return "sandbox";
}

void DropboxRequest::readyForRead()
{

}

void DropboxRequest::replyFinished()
{

}

DropboxRequest::~DropboxRequest()
{

}

DropboxUploadRequest::DropboxUploadRequest (Dropbox* dropbox, const QString& localFileName, const QString& remoteFilePath) :
    m_file (localFileName)
{
    m_dropbox = dropbox;
    if (!m_file.open (QIODevice::ReadOnly)) {
        m_error = FileError;
        QTimer::singleShot (0, this, SIGNAL (finished()));
        return;
    }

    m_buffer.open (QBuffer::ReadWrite);

    QString surl;
    surl = "https://api-content.dropbox.com/1/files_put/%1/%2";
    QUrl url (surl.arg (getRootType()).arg (remoteFilePath));
    sendRequest (url, QOAuth::PUT, &m_file);
}

void DropboxUploadRequest::readyForRead()
{
    m_buffer.write (m_reply->readAll());
}


DropboxUploadRequest::~DropboxUploadRequest()
{
}

void DropboxUploadRequest::replyFinished()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        qDebug() << "Reponse error " << m_reply->errorString();
        m_error = NetworkError;
    }

    m_buffer.seek (0);
    // Lets print the HTTP PUT response.
    QVariant result = m_parser.parse (m_buffer.data());
    qDebug() << result;
    m_file.close();
    emit finished();
}

DropboxDownloadRequest::DropboxDownloadRequest (Dropbox* dropbox, const QString& remoteFilePath, const QString& localFileName) :
    m_file (localFileName)
{
    m_dropbox = dropbox;
    if (!m_file.open (QIODevice::WriteOnly)) {
        m_error = FileError;
        qDebug() << "Failed opening file for writing!";
        QTimer::singleShot (0, this, SIGNAL (finished()));
        return;
    }
    QString urlString;
    urlString = "https://api-content.dropbox.com/1/files/%1/%2";
    QUrl url (urlString.arg (getRootType()).arg (remoteFilePath));
    sendRequest (url, QOAuth::GET);
}

DropboxDownloadRequest::~DropboxDownloadRequest()
{
}

void DropboxDownloadRequest::readyForRead()
{
    m_file.write (m_reply->readAll());
}

void DropboxDownloadRequest::replyFinished()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        qDebug() << "Reponse error " << m_reply->errorString();
        m_error = NetworkError;
    }
    m_file.close();
    emit finished();
}

DropboxCopyRequest::DropboxCopyRequest (Dropbox* dropbox, const QString& fromPath, const QString& toPath)
{
    m_dropbox = dropbox;
    QOAuth::ParamMap paramMap;
    paramMap.clear();
    paramMap.insert ("from_path", fromPath.toUtf8());
    paramMap.insert ("to_path", toPath.toUtf8());
    paramMap.insert ("root", getRootType().toUtf8());
    QUrl url ("https://api.dropbox.com/1/fileops/copy");
    m_buffer.open (QIODevice::ReadWrite);
    sendRequest (url, QOAuth::POST, NULL, paramMap);
}

void DropboxCopyRequest::readyForRead()
{
    m_buffer.write (m_reply->readAll());
}

void DropboxCopyRequest::replyFinished()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        m_error = NetworkError;
        qDebug() << "Reponse error " << m_reply->errorString();
        return ;
    }
    QVariant result = m_parser.parse (m_buffer.data());
    qDebug() << result;
    emit finished();
}

DropboxCopyRequest::~DropboxCopyRequest()
{
    m_buffer.close();
}

DropboxMoveRequest::DropboxMoveRequest (Dropbox* dropbox, const QString& fromPath, const QString& toPath)
{
    m_dropbox = dropbox;
    QOAuth::ParamMap paramMap;
    paramMap.clear();
    paramMap.insert ("from_path", fromPath.toUtf8());
    paramMap.insert ("to_path", toPath.toUtf8());
    paramMap.insert ("root", getRootType().toUtf8());
    QUrl url ("https://api.dropbox.com/1/fileops/move");
    m_buffer.open (QIODevice::ReadWrite);
    sendRequest (url, QOAuth::POST, NULL, paramMap);
}

void DropboxMoveRequest::readyForRead()
{
    m_buffer.write (m_reply->readAll());
}

void DropboxMoveRequest::replyFinished()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        m_error = NetworkError;
        qDebug() << "Reponse error " << m_reply->errorString();
        return ;
    }
    QVariant result = m_parser.parse (m_buffer.data());
    qDebug() << result;
    emit finished();
}

DropboxMoveRequest::~DropboxMoveRequest()
{
    m_buffer.close();
}

DropboxCreateFolderRequest::DropboxCreateFolderRequest (Dropbox* dropbox, const QString& path)
{
    m_dropbox = dropbox;
    QOAuth::ParamMap paramMap;
    paramMap.clear();
    paramMap.insert ("root", getRootType().toUtf8());
    paramMap.insert ("path", path.toUtf8());
    QUrl url ("https://api.dropbox.com/1/fileops/create_folder");
    m_buffer.open (QIODevice::ReadWrite);
    sendRequest (url, QOAuth::POST, NULL, paramMap);
}

void DropboxCreateFolderRequest::readyForRead()
{
    m_buffer.write (m_reply->readAll());
}

void DropboxCreateFolderRequest::replyFinished()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        m_error = NetworkError;
        qDebug() << "Reponse error " << m_reply->errorString();
    }
    QVariant result = m_parser.parse (m_buffer.data());
    qDebug() << result;
    emit finished();
}

DropboxCreateFolderRequest::~DropboxCreateFolderRequest()
{
    m_buffer.close();
}

DropboxDeleteRequest::DropboxDeleteRequest (Dropbox* dropbox, const QString& path)
{
    m_dropbox = dropbox;
    QOAuth::ParamMap paramMap;
    paramMap.clear();
    paramMap.insert ("root", getRootType().toUtf8());
    paramMap.insert ("path", path.toUtf8());
    QUrl url ("https://api.dropbox.com/1/fileops/delete");
    m_buffer.open (QIODevice::ReadWrite);
    sendRequest (url, QOAuth::POST, NULL, paramMap);
}

void DropboxDeleteRequest::readyForRead()
{
    m_buffer.write (m_reply->readAll());
}

void DropboxDeleteRequest::replyFinished()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        m_error = NetworkError;
        qDebug() << "Reponse error " << m_reply->errorString();
    }
    QVariant result = m_parser.parse (m_buffer.data());
    qDebug() << result;
    emit finished();
}

DropboxDeleteRequest::~DropboxDeleteRequest()
{
    m_buffer.close();
}


