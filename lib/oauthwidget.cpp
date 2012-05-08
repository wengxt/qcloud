#include <QtCore/QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtGui/QMessageBox>

#include "oauthwidget.h"
#include "ibackend.h"
#include "qcloud_utils.h"
#include "oauthwidget_p.h"
#include "oauthbackend.h"
#include "ui_oauthwidget.h"

namespace QCloud
{

OAuthWidgetPrivate::OAuthWidgetPrivate (OAuthWidget* parent) : QObject (parent)
    , p (parent)
    , backend (0)
    , ui (new Ui::OAuthWidget)
{
}

OAuthWidgetPrivate::~OAuthWidgetPrivate()
{
    delete ui;
}

void OAuthWidgetPrivate::urlChanged (const QUrl& url)
{
    qDebug() << url;
    if (QCloud::isCustomCallbackUrl (url)) {
        authorizeSuccess(url);
    }
}

void OAuthWidgetPrivate::loadStarted()
{
    ui->progressBar->show();
}

void OAuthWidgetPrivate::loadProgress (int p)
{
    ui->progressBar->setValue (p);
}

void OAuthWidgetPrivate::loadFinished (bool suc)
{
    Q_UNUSED (suc);
    ui->progressBar->hide();
}

void OAuthWidgetPrivate::authorizeSuccess(const QUrl& url)
{
    backend->setAuthUrl(url);
    ui->webView->disconnect (SIGNAL (urlChanged (QUrl)));
    emit p->authFinished (true);
}

OAuthWidget::OAuthWidget (QCloud::OAuthBackend* backend, QWidget* parent) : AuthWidget (parent)
    , d (new OAuthWidgetPrivate (this))
{
    d->ui->setupUi (this);
    d->ui->progressBar->hide();
    d->backend = backend;
    QWebView* webView = d->ui->webView;
    webView->settings()->globalSettings()->setAttribute (QWebSettings::LocalStorageEnabled, true);
    webView->settings()->globalSettings()->setAttribute (QWebSettings::OfflineStorageDatabaseEnabled, true);
    webView->settings()->globalSettings()->setAttribute (QWebSettings::JavascriptCanOpenWindows, true);
    webView->settings()->globalSettings()->setAttribute (QWebSettings::JavascriptCanAccessClipboard, true);
    webView->settings()->globalSettings()->setAttribute (QWebSettings::JavascriptEnabled, true);
    webView->page()->setNetworkAccessManager (backend->networkAccessManager());

    connect (webView, SIGNAL (loadStarted()), d, SLOT (loadStarted()));
    connect (webView, SIGNAL (loadProgress (int)), d, SLOT (loadProgress (int)));
    connect (webView, SIGNAL (loadFinished (bool)), d, SLOT (loadFinished (bool)));
}

OAuthWidget::~OAuthWidget()
{
}

void OAuthWidget::openUrl (const QUrl& url)
{
    d->ui->webView->disconnect (SIGNAL (urlChanged (QUrl)));
    d->ui->webView->load (url);
    connect (d->ui->webView, SIGNAL (urlChanged (QUrl)), d, SLOT (urlChanged (QUrl)));
}

void OAuthWidget::startAuth()
{
    d->backend->startAuth (this);
}


}
