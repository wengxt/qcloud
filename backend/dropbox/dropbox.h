#ifndef DROPBOX_H
#define DROPBOX_H

#include <QFile>
#include "oauthbackend.h"

namespace QJson
{
class Parser;
}

namespace QCloud
{
class App;
}

class Dropbox : public QCloud::OAuthBackend
{
    Q_OBJECT
public:
    explicit Dropbox (QObject* parent = 0);
    virtual ~Dropbox();
    virtual void setApp (QCloud::App* app);
    virtual bool authorize (QWidget* parent = 0);
    virtual bool uploadFile (const QString& filename, const QString& filepath);
    virtual bool downloadFile (const QString& remoteFilePath,const QString& localFileName);
    virtual void startAuth (QCloud::OAuthWidget* widget);
    virtual void loadAccountInfo();
    virtual void saveAccountInfo();
    
private slots:
    void httpGetReadyRead();

private:
    QFile* file;
    QNetworkReply* reply;
    QJson::Parser* m_parser;
    QByteArray m_uid;
    bool m_globalAccess;
};

#endif
