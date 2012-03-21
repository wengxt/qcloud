#ifndef QCLOUD_AUTHORIZEWIDGET_H
#define QCLOUD_AUTHORIZEWIDGET_H

#include <QWidget>
#include <QUrl>
#include "qcloud_global.h"

class QNetworkAccessManager;
class QPushButton;
class QWebView;
namespace Ui {
class AuthorizeWidget;
}

namespace QCloud {

class IBackend;

class QCLOUD_EXPORT AuthorizeWidget : public QWidget {
    Q_OBJECT
public:
    explicit AuthorizeWidget(IBackend* backend, QNetworkAccessManager* manager, QWidget* parent = 0);
    virtual ~AuthorizeWidget();
    void openUrl(const QUrl& url);
protected slots:
    void urlChanged(const QUrl& url);
    void authorizeButtonClicked();
    void authorizeSuccess();

protected:
    IBackend* m_backend;
    Ui::AuthorizeWidget* m_ui;
    QWebView* m_webView;
    QPushButton* m_authorizeButton;
    QNetworkAccessManager* m_networkaccessmanager;

};

}

#endif