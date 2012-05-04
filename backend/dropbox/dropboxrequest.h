#ifndef DROPBOXREQUREST_H
#define DROPBOXREQUREST_H

#include <QFile>
#include <QBuffer>
#include <qjson/parser.h>
#include <QtOAuth/QtOAuth>
#include <QNetworkRequest>
#include "request.h"

class QNetworkReply;
class QFile;

class Dropbox;
class DropboxRequest : public QCloud::Request
{
    Q_OBJECT
public:
    //virtual DropboxRequest(Dropbox* dropbox,const QString& ,const QString& );
    virtual ~DropboxRequest();
    virtual QCloud::Request::Error error();
    
protected slots:
    virtual void readyForRead();
    virtual void replyFinished();
protected:
    void sendRequest(const QUrl& url,const QOAuth::HttpMethod& method,const QOAuth::ParamMap* paramMap,QIODevice* device);
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
private slots:
    virtual void readyForRead();
    virtual void replyFinished();
private:
    QJson::Parser m_parser;
    QFile m_file;
};

class DropboxCopyRequest : public DropboxRequest
{
    Q_OBJECT
public:
    DropboxCopyRequest (Dropbox* dropbox, const QString& fromPath,const QString& toPath);
    virtual ~DropboxCopyRequest();
private slots:
    virtual void readyForRead();
    virtual void replyFinished();
private:
    QJson::Parser m_parser;
    QBuffer m_buffer;
};

#endif
