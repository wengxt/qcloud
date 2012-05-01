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

    // override IBackend
    virtual void setNetworkAccessManager (QNetworkAccessManager* manager);
    virtual void loadAccountInfo (const QString& key, QSettings& settings, ISecureStore* secureStore);
    virtual void saveAccountInfo (const QString& key, QSettings& settings, ISecureStore* secureStore);

    virtual void startAuth (QCloud::OAuthWidget* oauthWidget) = 0;
    void setAuthUrl (const QUrl& url);

protected:
    virtual bool requestToken();
    virtual QOAuth::ParamMap accessToken(bool* ok = 0, QOAuth::ParamMap params = QOAuth::ParamMap());

    void setRequestTokenUrl(const QString& url);
    void setAccessTokenUrl(const QString& url);

    QString appKey() const;
    void setAppKey (const QString& appkey);
    QString appSecret() const;
    void setAppSecret (const QString& appsecret);
    QByteArray oauthToken() const;
    QByteArray oauthTokenSecret() const;
    uint timeout() const;

protected:
    QByteArray authorizationHeader(const QUrl& url, QOAuth::HttpMethod method, QOAuth::ParamMap params = QOAuth::ParamMap());

private:
    class Private;
    Private* d;
};

}

#endif // IBACKEND_H
