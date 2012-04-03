#include <QtOAuth/QtOAuth>
#include <QUrl>
#include "dropbox.h"
#include "app.h"
#include "oauthwidget.h"
#include "authdialog.h"
#include "utils.h"

Dropbox::Dropbox (QObject* parent) : OAuthBackend (parent)
{
    m_requestTokenUrl = "https://api.dropbox.com/1/oauth/request_token";
    m_authorizeUrl = "https://www.dropbox.com/1/oauth/authorize";
    m_accessTokenUrl = "https://api.dropbox.com/1/oauth/access_token";
    // setAppKey("d2anwsztkcu1upz");
    // setAppSecret("kt9a7tuph615hzs");
}

Dropbox::~Dropbox()
{
}

void Dropbox::setApp (QCloud::App* app)
{
    QString key = app->settings().value ("Dropbox/AppKey").toString();
    QString secret = app->settings().value ("Dropbox/AppSecret").toString();
    if (!key.isEmpty() && !secret.isEmpty()) {
        setAppKey (key);
        setAppSecret (secret);
    }
}

bool Dropbox::authorize (QWidget* parent)
{
    if (!prepare())
        return false;

    QCloud::AuthDialog dialog (new QCloud::OAuthWidget (this), parent);
    dialog.setWindowTitle(tr("Dropbox Authentication"));
    dialog.setWindowState (Qt::WindowMaximized);
    int result = dialog.exec();
    return result == QDialog::Accepted;
}

void Dropbox::startAuth (QCloud::OAuthWidget* widget)
{
    QUrl url ("https://www.dropbox.com/1/oauth/authorize");
    url.addQueryItem ("oauth_token", m_oauthToken);
    url.addQueryItem ("oauth_callback", QCloud::customCallbackUrl().toString());

    widget->openUrl (url);
}


bool Dropbox::uploadFile (const QString& filename)
{
    return false;
}
