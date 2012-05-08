#ifndef QCLOUD_AUTHORIZEWIDGET_H
#define QCLOUD_AUTHORIZEWIDGET_H

#include <QtCore/QUrl>
#include <QtGui/QWidget>
#include <QtCloud/AuthWidget>

#include "qcloud_global.h"

class QNetworkAccessManager;

namespace QCloud
{

class OAuthBackend;
class OAuthWidgetPrivate;

class QCLOUD_EXPORT OAuthWidget : public AuthWidget
{
    Q_OBJECT
public:
    explicit OAuthWidget (OAuthBackend* backend, QWidget* parent = 0);
    virtual ~OAuthWidget();
    void openUrl (const QUrl& url);
    virtual void startAuth();
protected:
    friend class OAuthWidgetPrivate;
private:
    OAuthWidgetPrivate* d;
};

}

#endif
