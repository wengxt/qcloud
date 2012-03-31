#include <QtCore/QDebug>
#include "oauthbackend.h"

namespace QCloud
{

OAuthBackend::OAuthBackend (QObject* parent) : IBackend (parent)
    , m_oauth (new QOAuth::Interface (this))
{
}

OAuthBackend::~OAuthBackend()
{

}

bool OAuthBackend::accessToken()
{
    return false;
}

QString OAuthBackend::appKey() const
{
    return m_appKey;
}

QString OAuthBackend::appSecret() const
{
    return m_appSecret;
}

int OAuthBackend::error() const
{
    return 0;
}

QString OAuthBackend::oauthToken() const
{
    return m_oauthToken;
}

QString OAuthBackend::oauthTokenSecret() const
{
    return m_oauthTokenSecret;
}

bool OAuthBackend::requestToken()
{
    m_oauth->setRequestTimeout (timeout());
    m_oauth->setConsumerKey (appKey().toAscii());
    m_oauth->setConsumerSecret (appSecret().toAscii());

    QOAuth::ParamMap map = m_oauth->requestToken (requestTokenUrl(), QOAuth::POST, QOAuth::HMAC_SHA1);

    if (m_oauth->error() == 200) {
        qDebug() << map;
        m_oauthToken = map.value (QOAuth::tokenParameterName());
        m_oauthTokenSecret = map.value (QOAuth::tokenSecretParameterName());
        return true;
    }

    else {
        return false;
    }
}

QString OAuthBackend::requestTokenUrl() const
{
    return m_requestTokenUrl;
}

void OAuthBackend::setAppKey (const QString& appkey)
{
    m_appKey = appkey;
}

void OAuthBackend::setAppSecret (const QString& appsecret)
{
    m_appSecret = appsecret;
}


uint OAuthBackend::timeout() const
{
    return 0;
}

void OAuthBackend::setNetworkAccessManager (QNetworkAccessManager* manager)
{
    IBackend::setNetworkAccessManager (manager);
    m_oauth->setNetworkAccessManager (manager);
}

bool OAuthBackend::prepare()
{
    return requestToken();
}


}
