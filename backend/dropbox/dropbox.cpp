#include <QtOAuth/QtOAuth>
#include <QUrl>
#include "dropbox.h"
#include "authorizewidget.h"
#include "utils.h"

Dropbox::Dropbox(QObject* parent): OAuthBackend(parent)
{
    setObjectName("dropbox");
    m_requestTokenUrl = "https://api.dropbox.com/1/oauth/request_token";
    m_authorizeUrl = "https://www.dropbox.com/1/oauth/authorize";
    m_accessTokenUrl = "https://api.dropbox.com/1/oauth/access_token";
    m_name = tr("Dropbox");
    m_iconName = "qcloud-dropbox";
}

Dropbox::~Dropbox()
{

}

void Dropbox::authorize()
{
    if (m_authWidget) {
        QUrl url("https://www.dropbox.com/1/oauth/authorize");
        url.addQueryItem("oauth_token", m_oauthToken);
        url.addQueryItem("oauth_callback", QCloud::customCallbackUrl().toString());
        m_authWidget->openUrl(url);
    }
}

bool Dropbox::uploadFile (const QString& filename)
{
    return false;
}


Q_EXPORT_PLUGIN2(qcloud_dropbox, Dropbox)
