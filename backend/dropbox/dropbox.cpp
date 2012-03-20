#include <QtOAuth/QtOAuth>
#include <QUrl>
#include "dropbox.h"
#include <authorizewidget.h>

Dropbox::Dropbox(QObject* parent): IBackend(parent)
{
    m_requestTokenUrl = "https://api.dropbox.com/1/oauth/request_token";
    m_authorizeUrl = "https://www.dropbox.com/1/oauth/authorize";
    m_accessTokenUrl = "https://api.dropbox.com/1/oauth/access_token";
}

Dropbox::~Dropbox()
{

}

bool Dropbox::customAouth(const QString& userName, const QString& password)
{
    return false;
}

void Dropbox::authorize()
{
    if (m_authWidget) {
        QUrl url("https://www.dropbox.com/1/oauth/authorize");
        url.addQueryItem("oauth_token", m_oauthToken);
        url.addQueryItem("oauth_callback", "qcloud://custom");
        m_authWidget->openUrl(url);
    }
}



Q_EXPORT_PLUGIN2(qcloud_dropbox, Dropbox)
