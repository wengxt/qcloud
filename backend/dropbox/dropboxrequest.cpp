#include <QFile>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QTimer>
#include <QLocalSocket>
#include <qjson/parser.h>
#include "dropboxrequest.h"
#include "dropbox.h"
#include <qcloud_utils.h>

#define BUF_SIZE 512

inline static QString removeInvalidSlash(const QString& str)
{
    QString retString = "";
    if (str.isEmpty())
        return retString;
    retString += str[0];
    for (int i = 1; i < str.size(); i ++){
        if (str[i]!='/' || str[i - 1]!='/')
            retString += str[i];
    }
    if (retString[0] == '/')
        retString.remove(0,1);
    int pos = retString.size() - 1;
    if (pos <= 0)
        return retString;
    if (retString[pos] == '/')
        retString.remove(pos, 1);
    return retString;
}

QCloud::Request::Error DropboxRequest::error()
{
    return m_error;
}

void DropboxRequest::sendRequest (const QUrl& url, const QOAuth::HttpMethod& method, QIODevice* device, QOAuth::ParamMap paramMap)
{
    m_error = NoError;
    m_reply = 0;
    qDebug() << "Request URL" << url;
    QNetworkRequest request (url);
    if (method == QOAuth::POST) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        for (QOAuth::ParamMap::iterator it = paramMap.begin(); it != paramMap.end(); it++) {
            it.value() = QUrl::toPercentEncoding(QString::fromUtf8(it.value()));
        }
        qDebug() << paramMap;
        request.setRawHeader ("Authorization", m_dropbox->authorizationString (url, method, paramMap));
        qDebug() << m_dropbox->authorizationString (url, method, paramMap, QOAuth::ParseForRequestContent);
        m_reply = m_dropbox->networkAccessManager()->post (request, m_dropbox->inlineString (paramMap, QOAuth::ParseForRequestContent));
    } else {
        qDebug() << "Authorization String" << m_dropbox->authorizationString (url, method);
        request.setRawHeader ("Authorization", m_dropbox->authorizationString (url, method));
        if (method == QOAuth::GET)
            m_reply = m_dropbox->networkAccessManager()->get (request);
        else if (method == QOAuth::PUT) {
            m_reply = m_dropbox->networkAccessManager()->put (request, device);
        }
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

DropboxUploadRequest::DropboxUploadRequest (Dropbox* dropbox, const QString& localFileName, uint type, const QString& remoteFilePath) :
    m_iodevice(0)
{
    qDebug() << localFileName << type << remoteFilePath;
    switch(type) {
        case QCloud::IBackend::LocalFile:
        {
            QFile* file = new QFile(localFileName);
            m_iodevice = file;
            if (!file->open (QIODevice::ReadOnly)) {
                m_error = FileError;
                QTimer::singleShot (0, this, SIGNAL (finished()));
                return;
            }
        }
        break;
        case QCloud::IBackend::LocalSocket:
        {
            QLocalSocket* socket = new QLocalSocket;
            socket->connectToServer(localFileName, QIODevice::ReadOnly);
            if (!socket->waitForConnected()) {
                m_error = FileError;
                QTimer::singleShot (0, this, SIGNAL (finished()));
                return;
            }
            m_iodevice = socket;
        }
        break;
    }
    m_dropbox = dropbox;

    m_buffer.open (QBuffer::ReadWrite);

    QString surl("https://api-content.dropbox.com/1/files_put/%1/%2");
    surl = surl.arg (getRootType(),removeInvalidSlash(remoteFilePath));
    QUrl url (surl);
    sendRequest (url, QOAuth::PUT, m_iodevice);

    connect (m_reply, SIGNAL (uploadProgress(qint64, qint64)), this, SIGNAL(uploadProgress(qint64, qint64)));
    connect (m_reply, SIGNAL (downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
}


void DropboxUploadRequest::readyForRead()
{
    m_buffer.write (m_reply->readAll());
}


DropboxUploadRequest::~DropboxUploadRequest()
{
    if (m_iodevice)
        delete m_iodevice;
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
    qDebug() << "====Upload====";
    qDebug() << result;
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(m_iodevice);
    if (socket) {
        socket->disconnectFromServer();
    }
    else {
        m_iodevice->close();
    }
    emit finished();
}

DropboxDownloadRequest::DropboxDownloadRequest (Dropbox* dropbox, const QString& remoteFilePath, const QString& localFileName, uint type) :
    m_iodevice(0)
{
    qDebug() << "DropboxDownloadRequest" << remoteFilePath << localFileName << type;
    m_dropbox = dropbox;
    switch(type) {
        case QCloud::IBackend::LocalFile:
        {
            QFile* file = new QFile(localFileName);
            m_iodevice = file;
            if (!file->open (QIODevice::ReadOnly)) {
                m_error = FileError;
                QTimer::singleShot (0, this, SIGNAL (finished()));
                return;
            }
        }
        break;
        case QCloud::IBackend::LocalSocket:
        {
            QLocalSocket* socket = new QLocalSocket;
            socket->connectToServer(localFileName, QIODevice::WriteOnly);
            if (!socket->waitForConnected()) {
                m_error = FileError;
                QTimer::singleShot (0, this, SIGNAL (finished()));
                return;
            }
            m_iodevice = socket;
        }
        break;
    }
    QString surl("https://api-content.dropbox.com/1/files/%1/%2");
    surl = surl.arg (getRootType(),removeInvalidSlash(remoteFilePath));
    QUrl url (surl);
    sendRequest (url, QOAuth::GET);
    connect (m_reply, SIGNAL (uploadProgress(qint64,qint64)), this, SIGNAL(uploadProgress(qint64, qint64)));
    connect (m_reply, SIGNAL (downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
}

DropboxDownloadRequest::~DropboxDownloadRequest()
{
    if (m_iodevice)
        delete m_iodevice;
}

void DropboxDownloadRequest::readyForRead()
{
    QByteArray buf = m_reply->readAll();
    qint64 result = m_iodevice->write (buf);
    if (result < 0)
        qDebug() << "Download send error" << m_iodevice->errorString();

}

void DropboxDownloadRequest::replyFinished()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        qDebug() << "Reponse error " << m_reply->errorString();
        m_error = NetworkError;
    }
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(m_iodevice);
    if (socket) {
        if (socket->bytesToWrite()) {
            connect(m_iodevice, SIGNAL(bytesWritten(qint64)), SLOT(bytesWritten(qint64)));
        }
        else {
            socket->disconnectFromServer();
            emit finished();
        }
    }
    else {
        m_iodevice->close();
        emit finished();
    }
}


void DropboxDownloadRequest::bytesWritten (qint64)
{
    if (!m_iodevice->bytesToWrite()) {
        QLocalSocket* socket = qobject_cast<QLocalSocket*>(m_iodevice);
        socket->disconnectFromServer();
        emit finished();
    }
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
    }
    QVariant result = m_parser.parse (m_buffer.data());
    qDebug() << result;

    QVariantMap map = qvariant_cast<QVariantMap>(result);
    if (!map.isEmpty() && map.contains("error")) {
        QString err = map["error"].toString();
        qDebug() << err;

        /*
         * See this for more info:
         * https://www.dropbox.com/developers/reference/api#fileops-copy
         *
         * 403: Shared folders within app folders are not allowed by dropbox.
         * 406: We just move a file to another, can meet this error.
         *
         * So don't need to check these situations.
         */
        if (err.contains("already exists"))
            m_error = Request::FileExistsError;
        else if (err.contains ("not found"))
            m_error = Request::FileNotFoundError;
        else
            m_error = Request::OtherError;
    }
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

QCloud::EntryInfo DropboxGetInfoRequest::getInfoFromMap(const QVariantMap& infoMap)
{
    QCloud::EntryInfo info;
    qDebug() << infoMap["modified"].toString() << infoMap["modified"].toDateTime() << QDateTime::fromString(infoMap["modified"].toString(), Qt::ISODate);
    info.setValue(QCloud::EntryInfo::SizeType,infoMap["bytes"]);
    info.setValue(QCloud::EntryInfo::DirType, infoMap["is_dir"]);
    info.setValue(QCloud::EntryInfo::HashType,infoMap["hash"]);
    info.setValue(QCloud::EntryInfo::IconType,Dropbox::mapIcon(infoMap["icon"].toString(), info.isDir()));
    info.setValue(QCloud::EntryInfo::PathType,infoMap["path"]);
    info.setValue(QCloud::EntryInfo::ModifiedTimeType, QCloud::parseRFC2822Date(infoMap["modified"].toString()));
    info.setValue(QCloud::EntryInfo::MimeType,info.isDir() ? "inode/directory" : infoMap["mime_type"]);
    return info;
}

DropboxGetInfoRequest::DropboxGetInfoRequest(Dropbox* dropbox, const QString& path, QCloud::EntryInfo* info, QCloud::EntryInfoList* contents)
{
    m_dropbox = dropbox;
    m_info = info;
    m_contents = contents;
    QString cleanPath = removeInvalidSlash(path);
    if (!cleanPath.isEmpty())
        cleanPath += '/';
    QString urlString = "https://api.dropbox.com/1/metadata/%1/%2";
    urlString = urlString.arg(getRootType()).arg(cleanPath);
    m_buffer.open(QIODevice::ReadWrite);
    sendRequest(QUrl(urlString),QOAuth::GET);
}

void DropboxGetInfoRequest::readyForRead()
{
    m_buffer.write(m_reply->readAll());
}

void DropboxGetInfoRequest::replyFinished()
{
    if (m_reply->error() != QNetworkReply::NoError){
        m_error = NetworkError;
        qDebug() << "Reponse error " << m_reply->errorString();
    }
    QVariant result = m_parser.parse(m_buffer.data());
    QVariantMap infoMap = result.toMap();
    if (m_info!=NULL){
        (*m_info) = getInfoFromMap(infoMap);
    }
    if (m_contents){
        if (infoMap["is_dir"].toBool()==false)
            qDebug() << "Not a directory!";
        else{
            QCloud::EntryInfoList infoList;
            infoList.clear();
            QVariantList contentsList = infoMap["contents"].toList();
            foreach(QVariant i,contentsList){
                QCloud::EntryInfo info = getInfoFromMap(i.toMap());
                infoList << info;
            }
            (*m_contents) = infoList;
        }
    }
    emit finished();
}

DropboxGetInfoRequest::~DropboxGetInfoRequest()
{
    m_buffer.close();
}

