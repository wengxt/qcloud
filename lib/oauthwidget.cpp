#include <QtCore/QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtGui/QMessageBox>

#include "oauthwidget.h"
#include "ibackend.h"
#include "utils.h"
#include "oauthwidget_p.h"
#include "ui_oauthwidget.h"

namespace QCloud {

OAuthWidget::Private::Private(OAuthWidget* parent) : QObject(parent)
    ,p(parent)
    ,m_backend(0)
    ,m_ui(new Ui::OAuthWidget)
{
}

OAuthWidget::Private::~Private()
{
    delete m_ui;
}


void OAuthWidget::Private::authorizeButtonClicked()
{
    if (m_backend->prepare()) {
        m_backend->authorize();
    }
}

void OAuthWidget::Private::urlChanged(const QUrl& url)
{
    qDebug() << url;
    if (QCloud::isCustomCallbackUrl(url)) {
        authorizeSuccess();
    }
}

void OAuthWidget::Private::loadStarted()
{
    m_ui->progressBar->show();
}

void OAuthWidget::Private::loadProgress(int p)
{
    m_ui->progressBar->setValue(p);
}

void OAuthWidget::Private::loadFinished(bool suc)
{
    Q_UNUSED(suc);
    m_ui->progressBar->hide();
}

void OAuthWidget::Private::authorizeSuccess()
{
    m_ui->webView->disconnect(SIGNAL(urlChanged(QUrl)));
    QMessageBox::information(p, "Success", "Success");
}

OAuthWidget::OAuthWidget(IBackend* backend, QNetworkAccessManager* manager, QWidget* parent): QWidget(parent)
    ,d(new Private(this))
{
    d->m_ui->setupUi(this);
    d->m_ui->progressBar->hide();
    d->m_backend = backend;
    QWebView* webView = d->m_ui->webView;
    webView->settings()->globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    webView->settings()->globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    webView->settings()->globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    webView->settings()->globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    webView->settings()->globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    webView->page()->setNetworkAccessManager(manager);

    connect(d->m_ui->authorizeButton, SIGNAL(clicked(bool)), d, SLOT(authorizeButtonClicked()));
    connect(webView, SIGNAL(loadStarted()), d, SLOT(loadStarted()));
    connect(webView, SIGNAL(loadProgress(int)), d, SLOT(loadProgress(int)));
    connect(webView, SIGNAL(loadFinished(bool)), d, SLOT(loadFinished(bool)));

    backend->setAuthorizeWidget(this);
    backend->setNetworkAccessManager(manager);
}

OAuthWidget::~OAuthWidget()
{
}

void OAuthWidget::openUrl(const QUrl& url)
{
    d->m_ui->webView->disconnect(SIGNAL(urlChanged(QUrl)));
    d->m_ui->webView->load(url);
    connect(d->m_ui->webView, SIGNAL(urlChanged(QUrl)), d, SLOT(urlChanged(QUrl)));
}

}