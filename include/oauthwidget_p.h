#ifndef QCLOUD_OAUTHWIDGET_P_H
#define QCLOUD_OAUTHWIDGET_P_H

#include <QtCore/QObject>
#include "oauthwidget.h"

class QPushButton;
class QWebView;

namespace Ui
{
class OAuthWidget;
}

namespace QCloud
{
class OAuthWidget::Private : public QObject
{
    Q_OBJECT
    friend class OAuthWidget;
public:
    explicit Private (OAuthWidget* parent);
    virtual ~Private();
protected slots:
    void urlChanged (const QUrl& url);
    void authorizeSuccess(const QUrl& url);
    void loadStarted();
    void loadProgress (int);
    void loadFinished (bool suc);

protected:
    OAuthWidget* p;
    OAuthBackend* m_backend;
    Ui::OAuthWidget* m_ui;
};
}

#endif
