#ifndef DROPBOX_H
#define DROPBOX_H

#include <QFile>
#include "oauthbackend.h"

class DropboxDownloadRequest;
class DropboxUploadRequest;

namespace QCloud
{
class App;
class Request;
}

class Dropbox : public QCloud::OAuthBackend
{
    friend class DropboxUploadRequest;
    friend class DropboxDownloadRequest;
    Q_OBJECT
public:
    explicit Dropbox (QObject* parent = 0);
    virtual ~Dropbox();
    virtual void setApp (QCloud::App* app);
    virtual bool authorize (QWidget* parent = 0);
    virtual QCloud::Request* uploadFile (const QString& localFileName, const QString& remoteFilePath);
    virtual QCloud::Request* downloadFile (const QString& remoteFilePath,const QString& localFileName);
    virtual void startAuth (QCloud::OAuthWidget* widget);
    virtual void loadAccountInfo();
    virtual void saveAccountInfo();

protected:
    QByteArray m_uid;
    bool m_globalAccess;
};

#endif
