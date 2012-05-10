#ifndef DROPBOXREQUREST_H
#define DROPBOXREQUREST_H

#include <QFile>
#include <QBuffer>
#include <QVariant>
#include <QMap>
#include <qjson/parser.h>
#include <QtOAuth/QtOAuth>
#include "request.h"
#include "entryinfo.h"

class QNetworkReply;
class QFile;
class EntryInfo;

class Dropbox;
class DropboxRequest : public QCloud::Request
{
    Q_OBJECT
public:
    virtual ~DropboxRequest();
    virtual QCloud::Request::Error error();

protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    void sendRequest(const QUrl& url, const QOAuth::HttpMethod& method, QIODevice* device = 0, QOAuth::ParamMap paramMap = QOAuth::ParamMap());
    QString getRootType();
    Dropbox* m_dropbox;
    QCloud::Request::Error m_error;
    QNetworkReply* m_reply;
};

class DropboxUploadRequest : public DropboxRequest
{
    Q_OBJECT
public:
    DropboxUploadRequest (Dropbox* dropbox, const QString& localFileName, const QString& remoteFilePath);
    virtual ~DropboxUploadRequest();

protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    QJson::Parser m_parser;
    QFile m_file;
    QBuffer m_buffer;
};


class DropboxDownloadRequest : public DropboxRequest
{
    Q_OBJECT
public:
    DropboxDownloadRequest (Dropbox* dropbox, const QString& remoteFilePath, const QString& localFileName);
    virtual ~DropboxDownloadRequest();
protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    QJson::Parser m_parser;
    QFile m_file;
};

class DropboxCopyRequest : public DropboxRequest
{
    Q_OBJECT
public:
    DropboxCopyRequest (Dropbox* dropbox, const QString& fromPath,const QString& toPath);
    virtual ~DropboxCopyRequest();
protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    QJson::Parser m_parser;
    QBuffer m_buffer;
};

class DropboxMoveRequest : public DropboxRequest
{
    Q_OBJECT
public:
    DropboxMoveRequest (Dropbox* dropbox, const QString& fromPath,const QString& toPath);
    virtual ~DropboxMoveRequest();
protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    QJson::Parser m_parser;
    QBuffer m_buffer;
};

class DropboxCreateFolderRequest : public DropboxRequest
{
    Q_OBJECT
public:
    DropboxCreateFolderRequest (Dropbox* dropbox, const QString& path);
    virtual ~DropboxCreateFolderRequest();
protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    QJson::Parser m_parser;
    QBuffer m_buffer;
};

class DropboxDeleteRequest : public DropboxRequest
{
    Q_OBJECT
public:
    DropboxDeleteRequest (Dropbox* dropbox, const QString& path);
    virtual ~DropboxDeleteRequest();
protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    QJson::Parser m_parser;
    QBuffer m_buffer;
};

class DropboxGetInfoRequest : public DropboxRequest
{
    Q_OBJECT
public:
    DropboxGetInfoRequest (Dropbox* dropbox, const QString& path,QCloud::EntryInfo* info);
    virtual ~DropboxGetInfoRequest();
protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    QJson::Parser m_parser;
    QBuffer m_buffer;
    QCloud::EntryInfo* m_info;
private:
    QCloud::EntryInfo getInfoFromMap(const QVariantMap& infoMap);
};

#endif
