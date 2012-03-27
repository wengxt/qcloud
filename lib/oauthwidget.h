#ifndef QCLOUD_AUTHORIZEWIDGET_H
#define QCLOUD_AUTHORIZEWIDGET_H

#include <QtCore/QUrl>
#include <QtGui/QWidget>
#include "qcloud_global.h"
#include "authwidget.h"

class QNetworkAccessManager;

namespace QCloud {

class OAuthBackend;

class QCLOUD_EXPORT OAuthWidget : public AuthWidget {
    Q_OBJECT
public:
    explicit OAuthWidget(OAuthBackend* backend, QWidget* parent = 0);
    virtual ~OAuthWidget();
    void openUrl(const QUrl& url);
    virtual void startAuth();
private:
    class Private;
    Private* d;
};

}

#endif