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
class OAuthWidgetPrivate : public QObject
{
    Q_OBJECT
    friend class OAuthWidget;
public:
    explicit OAuthWidgetPrivate (OAuthWidget* parent);
    virtual ~OAuthWidgetPrivate();
protected slots:
    void urlChanged (const QUrl& url);
    void authorizeSuccess(const QUrl& url);
    void loadStarted();
    void loadProgress (int);
    void loadFinished (bool suc);

protected:
    OAuthWidget* p;
    OAuthBackend* backend;
    Ui::OAuthWidget* ui;
};
}

#endif
