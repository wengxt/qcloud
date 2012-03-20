#include <QDebug>
#include "ibackend.h"

namespace QCloud {
    
IBackend::IBackend(QObject* parent) : 
    m_oauth(new QOAuth::Interface(this))
{
}

IBackend::~IBackend()
{

}

bool IBackend::accessToken()
{
    return false;
}

QString IBackend::appKey() const
{
    return m_appKey;
}

QString IBackend::appSecret() const
{
    return m_appSecret;
}

int IBackend::error() const
{
    return 0;
}

QString IBackend::oauthToken() const
{
    return m_oauthToken;
}

QString IBackend::oauthTokenSecret() const
{
    return m_oauthTokenSecret;
}

bool IBackend::requestToken()
{
    m_oauth->setRequestTimeout( timeout() );
    m_oauth->setConsumerKey( appKey().toAscii() );
    m_oauth->setConsumerSecret( appSecret().toAscii() );

    QOAuth::ParamMap map = m_oauth->requestToken( requestTokenUrl(), QOAuth::POST, QOAuth::HMAC_SHA1 );

    if( m_oauth->error() == 200 )
    {
        qDebug() << map;
        m_oauthToken = map.value( QOAuth::tokenParameterName() );
        m_oauthTokenSecret = map.value( QOAuth::tokenSecretParameterName() );
        return true;
    }

    else
    {
        return false;
    }
}

QString IBackend::requestTokenUrl() const
{
    return m_requestTokenUrl;
}

void IBackend::setAppKey(const QString& appkey)
{
    m_appKey = appkey;
}

void IBackend::setAppSecret(const QString& appsecret)
{
    m_appSecret = appsecret;
}


uint IBackend::timeout() const
{
    return 0;
}

void IBackend::setAuthorizeWidget(AuthorizeWidget* widget)
{
    m_authWidget = widget;
}


    
}