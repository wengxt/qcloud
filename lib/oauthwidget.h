#ifndef QCLOUD_AUTHORIZEWIDGET_H
#define QCLOUD_AUTHORIZEWIDGET_H

#include <QtCore/QUrl>
#include <QtGui/QWidget>
#include "qcloud_global.h"

class QNetworkAccessManager;

namespace QCloud {

class IBackend;

class QCLOUD_EXPORT OAuthWidget : public QWidget {
    Q_OBJECT
public:
    explicit OAuthWidget(IBackend* backend, QNetworkAccessManager* manager, QWidget* parent = 0);
    virtual ~OAuthWidget();
    void openUrl(const QUrl& url);
private:
    class Private;
    Private* d;
};

}

#endif