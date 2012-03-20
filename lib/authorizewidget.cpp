#include <QNetworkRequest>
#include <QMessageBox>
#include <QDebug>

#include "authorizewidget.h"
#include "ibackend.h"
#include "networkaccessmanager.h"
#include "ui_authorizewidget.h"

namespace QCloud {

AuthorizeWidget::AuthorizeWidget(IBackend* backend, QWidget* parent): QWidget(parent)
    ,m_backend(backend)
    ,m_ui(new Ui::AuthorizeWidget)
{
    m_ui->setupUi(this);
    m_authorizeButton = m_ui->authorizeButton;
    m_webView = m_ui->webView;
    m_webView->settings()->globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    m_webView->settings()->globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    m_webView->settings()->globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    m_webView->settings()->globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    m_webView->settings()->globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    m_networkaccessmanager = new NetworkAccessManager(m_webView->page()->networkAccessManager(), this);
    m_webView->page()->setNetworkAccessManager(m_networkaccessmanager);
    
    connect(m_authorizeButton, SIGNAL(clicked(bool)), this, SLOT(authorizeButtonClicked()));
    
    backend->setAuthorizeWidget(this);
}

AuthorizeWidget::~AuthorizeWidget()
{
    delete m_ui;
}

void AuthorizeWidget::openUrl(const QUrl& url)
{
    m_webView->disconnect(SIGNAL(urlChanged(QUrl)));
    m_webView->load(url);
    connect(m_webView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
}

void AuthorizeWidget::authorizeButtonClicked()
{
    if (m_backend->requestToken()) {
        m_backend->authorize();
    }
}

void AuthorizeWidget::urlChanged(const QUrl& url)
{
    qDebug() << url;
    if (url.scheme() == "qcloud") {
        authorizeSuccess();
    }
}

void AuthorizeWidget::authorizeSuccess()
{
    m_webView->disconnect(SIGNAL(urlChanged(QUrl)));
    QMessageBox::information(this, "Success", "Success");
}
   
}