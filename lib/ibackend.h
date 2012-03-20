#ifndef IBACKEND_H
#define IBACKEND_H

#include <QtCore/QtPlugin>
#include <QtOAuth/QtOAuth>

namespace QCloud
{

class AuthorizeWidget;
class Q_DECL_EXPORT IBackend : public QObject
{
    Q_OBJECT
public:
    explicit IBackend(QObject* parent = 0);
    virtual ~IBackend();
    
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
    void setAuthorizeWidget(AuthorizeWidget* widget);
    
protected:
    QString m_appKey;
    QString m_appSecret;
    QString m_oauthToken;
    QString m_oauthTokenSecret;
    QString m_requestTokenUrl;
    QString m_authorizeUrl;
    QString m_accessTokenUrl;
    QOAuth::Interface* m_oauth;
    AuthorizeWidget* m_authWidget;
};

}

#endif // IBACKEND_H