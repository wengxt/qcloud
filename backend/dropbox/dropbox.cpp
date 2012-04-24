#include <QtOAuth/QtOAuth>
#include <QUrl>
#include <QLocale>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QFile>
#include <qjson/parser.h>
#include "dropbox.h"
#include "app.h"
#include "oauthwidget.h"
#include "authdialog.h"
#include "qcloud_utils.h"
#define BUFF_SIZE 10

Dropbox::Dropbox (QObject* parent) : OAuthBackend (parent)
    ,m_parser(new QJson::Parser)
    ,m_globalAccess(false)
{
    m_requestTokenUrl = "https://api.dropbox.com/1/oauth/request_token";
    m_authorizeUrl = "https://www.dropbox.com/1/oauth/authorize";
    m_accessTokenUrl = "https://api.dropbox.com/1/oauth/access_token";
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

    if (!prepare())
        return false;

    QCloud::AuthDialog dialog (new QCloud::OAuthWidget (this), parent);
    dialog.setWindowTitle(tr("Dropbox Authentication"));
    dialog.setWindowState (Qt::WindowMaximized);
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        QOAuth::ParamMap reply = m_oauth->accessToken("https://api.dropbox.com/1/oauth/access_token", QOAuth::POST, m_oauthToken, m_oauthTokenSecret);
        if (m_oauth->error() == QOAuth::NoError) {
            m_oauthToken = reply.value(QOAuth::tokenParameterName());
            m_oauthTokenSecret = reply.value(QOAuth::tokenSecretParameterName());
            m_uid = reply.value("uid");
            return true;
        }
    }
    return false;
}

void Dropbox::startAuth (QCloud::OAuthWidget* widget)
{
    QUrl url ("https://www.dropbox.com/1/oauth/authorize");
    url.addQueryItem ("oauth_token", m_oauthToken);
    url.addQueryItem ("oauth_callback", QCloud::customCallbackUrl().toString());

    widget->openUrl (url);
}


bool Dropbox::uploadFile (const QString& localFileName, const QString& remoteFilePath)
{
    QFile file(localFileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QString surl;
    if (m_globalAccess)
        surl = "https://api-content.dropbox.com/1/files_put/dropbox/%1";
    else
        surl = "https://api-content.dropbox.com/1/files_put/sandbox/%1";
    QUrl url(surl.arg(remoteFilePath));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", authorizationHeader(url, QOAuth::PUT));
    QNetworkReply* reply = m_networkAccessManager->put(request, &file);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();

    if (reply->error()!=QNetworkReply::NoError){
        qDebug() << "Reponse error " << reply->errorString();
        return false;
    }

    // Lets print the HTTP PUT response.
    QVariant result = m_parser->parse(reply->readAll());
    qDebug() << result;
    return true;
}

bool Dropbox::downloadFile (const QString& remoteFilePath,const QString& localFileName)
{
    QFile file(localFileName);
    if (!file.open(QIODevice::WriteOnly)){
        qDebug() << "Failed opening file for writing!";
        return false;
    }
    QString urlString;
    if (m_globalAccess)
        urlString = "https://api-content.dropbox.com/1/files/dropbox/%1";
    else
        urlString = "https://api-content.dropbox.com/1/files/sandbox/%1";
    QUrl url(urlString.arg(remoteFilePath));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization",authorizationHeader(url,QOAuth::GET));
    QNetworkReply* reply = m_networkAccessManager->get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    
    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();
    if (reply->error()!=QNetworkReply::NoError){
        qDebug() << "Reponse error " << reply->errorString();
        return false;
    }
    /*
    QByteArray buff;
    buff = reply->read(BUFF_SIZE);
    do{
        file.write(buff);
        buff = reply->read(BUFF_SIZE);
    }
    while (buff.size()!=0);
    */
    file.write(reply->readAll());
    file.close();
    
    return true;
}

void Dropbox::loadAccountInfo()
{
    QUrl url("https://api.dropbox.com/1/account/info");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", authorizationHeader(url, QOAuth::GET));
    QNetworkReply* reply = m_networkAccessManager->get(request);
    
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    
    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();
    
    // Lets print the HTTP GET response.
    qDebug() << QString::fromUtf8(reply->readAll());
}

void Dropbox::saveAccountInfo()
{
    
}
