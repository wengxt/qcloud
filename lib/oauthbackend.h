#ifndef QCLOUD_OAUTHBACKEND_H
#define QCLOUD_OAUTHBACKEND_H

#include <QtCore/QtPlugin>
#include <QUrl>
#include <QtOAuth/QtOAuth>
#include "qcloud_global.h"
#include "ibackend.h"

namespace QCloud
{

class AuthorizeWidget;
class QCLOUD_EXPORT OAuthBackend : public IBackend
{
    Q_OBJECT
public:
    explicit OAuthBackend(QObject* parent = 0);
    virtual ~OAuthBackend();
    virtual void setNetworkAccessManager(QNetworkAccessManager* manager);
    virtual bool prepare();
    
    virtual bool requestToken();
    virtual void authorize() = 0;
    virtual bool accessToken();
    virtual int error() const;

    QString appKey() const;
    void setAppKey(const QString & appkey); 
    QString appSecret() const;
    void setAppSecret(const QString & appsecret);
    QString oauthToken() const;
    QString oauthTokenSecret() const;
    QString requestTokenUrl() const;
    uint timeout() const;
protected:
    QString m_appKey;
    QString m_appSecret;
    QString m_oauthToken;
    QString m_oauthTokenSecret;
    QString m_requestTokenUrl;
    QString m_authorizeUrl;
    QString m_accessTokenUrl;
    QOAuth::Interface* m_oauth;
};

}

#endif // IBACKEND_H