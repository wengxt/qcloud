#include <QtOAuth/QtOAuth>
#include <QUrl>
#include <QLocale>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QFile>
#include <QIcon>
#include "dropbox.h"
#include "dropboxrequest.h"
#include "app.h"
#include "oauthwidget.h"
#include "authdialog.h"
#include "qcloud_utils.h"
#include "request.h"
#include <isecurestore.h>

Dropbox::Dropbox (QObject* parent) : OAuthBackend (parent)
    ,m_globalAccess(false)
{
    setRequestTokenUrl("https://api.dropbox.com/1/oauth/request_token");
    setAccessTokenUrl("https://api.dropbox.com/1/oauth/access_token");
    setAppKey("d2anwsztkcu1upz");
    setAppSecret("kt9a7tuph615hzs");
}

Dropbox::~Dropbox()
{
}

void Dropbox::setApp (QCloud::App* app)
{
    m_app = app;
    QString key = app->settings()->value ("Dropbox/AppKey").toString();
    QString secret = app->settings()->value ("Dropbox/AppSecret").toString();
    m_globalAccess = app->settings()->value("Dropbox/GlobalAccess").toBool();
    if (!key.isEmpty() && !secret.isEmpty()) {
        setAppKey (key);
        setAppSecret (secret);
    }
}

bool Dropbox::authorize (QWidget* parent)
{
    if (!m_app)
        return false;

    if (!requestToken())
        return false;

    QCloud::AuthDialog dialog (new QCloud::OAuthWidget (this), parent);
    dialog.setWindowTitle(tr("Dropbox Authentication"));
    dialog.setWindowState (Qt::WindowMaximized);
    dialog.setWindowIcon(QIcon::fromTheme("qcloud_dropbox"));
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        bool ok = false;
        QOAuth::ParamMap reply = accessToken(&ok);
        if (ok) {
            m_uid = reply.value("uid");
            return true;
        }
    }
    return false;
}

void Dropbox::startAuth (QCloud::OAuthWidget* widget)
{
    QUrl url ("https://www.dropbox.com/1/oauth/authorize");
    url.addQueryItem ("oauth_token", oauthToken());
    url.addQueryItem ("oauth_callback", QCloud::customCallbackUrl().toString());

    widget->openUrl (url);
}


QCloud::Request* Dropbox::uploadFile (const QString& localFileName, const QString& remoteFilePath)
{
    return new DropboxUploadRequest(this, localFileName, remoteFilePath);
}

QCloud::Request* Dropbox::downloadFile (const QString& remoteFilePath,const QString& localFileName)
{
    return new DropboxDownloadRequest(this, remoteFilePath, localFileName);
}

QCloud::Request* Dropbox::copyFile(const QString& fromPath, const QString& toPath)
{
    return new DropboxCopyRequest(this, fromPath, toPath);
}

QCloud::Request* Dropbox::moveFile(const QString& fromPath, const QString& toPath)
{
    return new DropboxMoveRequest(this, fromPath, toPath);
}

QCloud::Request* Dropbox::createFolder(const QString& path)
{
    return new DropboxCreateFolderRequest(this, path);
}

QCloud::Request* Dropbox::deleteFile(const QString& path)
{
    return new DropboxDeleteRequest(this, path);
}

QString Dropbox::userName() {
    if (m_userName.isEmpty()) {
        QUrl url("https://api.dropbox.com/1/account/info");
        QNetworkRequest request(url);
        qDebug() << authorizationHeader(url, QOAuth::GET);
        request.setRawHeader("Authorization", authorizationHeader(url, QOAuth::GET));
        QNetworkReply* reply = m_networkAccessManager->get(request);

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));

        // Execute the event loop here, now we will wait here until readyRead() signal is emitted
        // which in turn will trigger event loop quit.
        loop.exec();

        // Lets print the HTTP GET response.
        QJson::Parser parser;
        QByteArray result = reply->readAll();
        qDebug() << result;
        QMap< QString, QVariant > var = parser.parse(result).toMap();
        QString name = var["display_name"].toString();
        QString email = var["email"].toString();
        if (!name.isNull() && !email.isNull()) {
            m_userName = QString(tr("%1 (%2)")).arg(name).arg(email);
        }
    }
    return m_userName;
}

void Dropbox::loadAccountInfo(const QString& key, QSettings& settings, QCloud::ISecureStore* securestore)
{
    OAuthBackend::loadAccountInfo(key, settings, securestore);
    m_userName = settings.value("UserName").toString();
}

void Dropbox::saveAccountInfo(const QString& key, QSettings& settings, QCloud::ISecureStore* securestore)
{
    OAuthBackend::saveAccountInfo(key, settings, securestore);
    settings.setValue("UserName", m_userName);
}
