#ifndef QCLOUD_OAUTHBACKEND_H
#define QCLOUD_OAUTHBACKEND_H

#include <QtCore/QtPlugin>
#include <QtCore/QUrl>
#include <QtOAuth/QtOAuth>
#include <QtCloud/IBackend>

#include "qcloud_global.h"

namespace QCloud
{

class OAuthWidget;
class QCLOUD_EXPORT OAuthBackend : public IBackend
{
    Q_OBJECT
public:
    explicit OAuthBackend (QObject* parent = 0);
    virtual ~OAuthBackend();
    virtual void setNetworkAccessManager (QNetworkAccessManager* manager);
    virtual bool prepare();

    virtual bool requestToken();
    virtual bool authorize (QWidget* widget = 0) = 0;
    virtual void startAuth (QCloud::OAuthWidget* oauthWidget) = 0;
    virtual bool accessToken();
    virtual int error() const;

    QString appKey() const;
    void setAppKey (const QString& appkey);
    QString appSecret() const;
    void setAppSecret (const QString& appsecret);
    QByteArray oauthToken() const;
    QByteArray oauthTokenSecret() const;
    QString requestTokenUrl() const;
    uint timeout() const;
    void setAuthUrl (const QUrl& url);

protected:
    QByteArray authorizationHeader(const QUrl& url, QOAuth::HttpMethod method, QOAuth::ParamMap params = QOAuth::ParamMap());

    QString m_appKey;
    QString m_appSecret;
    QByteArray m_oauthToken;
    QByteArray m_oauthTokenSecret;
    QString m_requestTokenUrl;
    QString m_authorizeUrl;
    QString m_accessTokenUrl;
    QUrl m_authUrl;
    QOAuth::Interface* m_oauth;
};

}

#endif // IBACKEND_H
