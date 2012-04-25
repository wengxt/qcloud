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

#define BUF_SIZE 512

DropboxUploadRequest::DropboxUploadRequest(Dropbox* dropbox, const QString& localFileName, const QString& remoteFilePath) : Request(dropbox)
    ,m_file(localFileName)
    ,m_error(NoError)
    ,m_reply(0)
{
    if (!m_file.open(QIODevice::ReadOnly)) {
        m_error = FileError;
        QTimer::singleShot(0, this, SIGNAL(finished()));
        return;
    }

    QString surl;
    if (dropbox->m_globalAccess)
        surl = "https://api-content.dropbox.com/1/files_put/dropbox/%1";
    else
        surl = "https://api-content.dropbox.com/1/files_put/sandbox/%1";
    QUrl url(surl.arg(remoteFilePath));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", dropbox->authorizationHeader(url, QOAuth::PUT));
    m_reply = dropbox->networkAccessManager()->put(request, &m_file);

    connect(m_reply, SIGNAL(finished()), this, SLOT(replyFinished()));

}

DropboxUploadRequest::~DropboxUploadRequest()
{
}

void DropboxUploadRequest::replyFinished()
{
    if (m_reply->error()!=QNetworkReply::NoError){
        qDebug() << "Reponse error " << m_reply->errorString();
        m_error = NetworkError;
    }

    // Lets print the HTTP PUT response.
    QVariant result = m_parser.parse(m_reply->readAll());
    qDebug() << result;
    m_file.close();
    emit finished();
}

QCloud::Request::Error DropboxUploadRequest::error()
{
    return NoError;
}

DropboxDownloadRequest::DropboxDownloadRequest(Dropbox* dropbox, const QString& remoteFilePath, const QString& localFileName) : QCloud::Request(dropbox)
    ,m_file(localFileName)
    ,m_error(NoError)
    ,m_reply(0)
{
    if (!m_file.open(QIODevice::WriteOnly)){
        m_error = FileError;
        qDebug() << "Failed opening file for writing!";
        QTimer::singleShot(0, this, SIGNAL(finished()));
        return;
    }
    QString urlString;
    if (dropbox->m_globalAccess)
        urlString = "https://api-content.dropbox.com/1/files/dropbox/%1";
    else
        urlString = "https://api-content.dropbox.com/1/files/sandbox/%1";
    QUrl url(urlString.arg(remoteFilePath));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization",dropbox->authorizationHeader(url,QOAuth::GET));
    m_reply = dropbox->networkAccessManager()->get(request);
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(readyForRead()));
    connect(m_reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

DropboxDownloadRequest::~DropboxDownloadRequest()
{
}

void DropboxDownloadRequest::readyForRead()
{
    char buf[BUF_SIZE];
    int size = 0;
    while ((size = m_reply->read(buf, BUF_SIZE)) > 0)
        m_file.write(buf, size);
}

void DropboxDownloadRequest::replyFinished()
{
    if (m_reply->error()!=QNetworkReply::NoError){
        qDebug() << "Reponse error " << m_reply->errorString();
        m_error = NetworkError;
    }
    m_file.close();
    emit finished();
}

QCloud::Request::Error DropboxDownloadRequest::error()
{
    return m_error;
}
