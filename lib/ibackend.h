#ifndef IBACKEND_H
#define IBACKEND_H

#include <QtCore/QtPlugin>
#include "qcloud_global.h"

class QNetworkAccessManager;
namespace QCloud
{

class AuthorizeWidget;
class QCLOUD_EXPORT IBackend : public QObject
{
    Q_OBJECT
public:
    explicit IBackend(QObject* parent = 0);
    const QString& name();
    const QString& iconName();
    virtual ~IBackend();

    virtual bool prepare();
    virtual void authorize() = 0;
    virtual void setNetworkAccessManager(QNetworkAccessManager* manager);
    virtual bool uploadFile(const QString& filename) = 0;

    void setAuthorizeWidget(AuthorizeWidget* widget);

protected:
    AuthorizeWidget* m_authWidget;
    QString m_name;
    QString m_iconName;
};

}

#endif // IBACKEND_H