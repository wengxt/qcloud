#ifndef DROPBOXREQUREST_H
#define DROPBOXREQUREST_H

#include <QFile>
#include <qjson/parser.h>
#include "request.h"

class QNetworkReply;
class QFile;

class Dropbox;
class DropboxUploadRequest : public QCloud::Request
{
    Q_OBJECT
public:
    DropboxUploadRequest(Dropbox* dropbox, const QString& localFileName, const QString& remoteFilePath);
    virtual ~DropboxUploadRequest();
    virtual QCloud::Request::Error error();

private slots:
    void replyFinished();
private:
    QJson::Parser m_parser;
    QFile m_file;
    QCloud::Request::Error m_error;
    QNetworkReply* m_reply;
};


class DropboxDownloadRequest : public QCloud::Request
{
    Q_OBJECT
public:
    DropboxDownloadRequest(Dropbox* dropbox, const QString& remoteFilePath,const QString& localFileName);
    virtual ~DropboxDownloadRequest();
    virtual QCloud::Request::Error error();
private slots:
    void readyForRead();
    void replyFinished();
private:
    QJson::Parser m_parser;
    QFile m_file;
    QCloud::Request::Error m_error;
    QNetworkReply* m_reply;
};

#endif