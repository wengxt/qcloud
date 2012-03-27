#ifndef QCLOUD_OAUTHWIDGET_P_H
#define QCLOUD_OAUTHWIDGET_P_H
#include <QObject>
#include "oauthwidget.h"

class QPushButton;
class QWebView;

namespace Ui {
class OAuthWidget;
}

namespace QCloud {
class OAuthWidget::Private : public QObject {
    Q_OBJECT
    friend class OAuthWidget;
public:
    explicit Private (OAuthWidget* parent);
    virtual ~Private();
protected slots:
    void urlChanged(const QUrl& url);
    void authorizeButtonClicked();
    void authorizeSuccess();
    void loadStarted();
    void loadProgress(int);
    void loadFinished(bool suc);

protected:
    OAuthWidget* p;
    IBackend* m_backend;
    Ui::OAuthWidget* m_ui;
};
}

#endif