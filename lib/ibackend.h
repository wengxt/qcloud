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
    virtual ~IBackend();
    
    virtual bool prepare();
    virtual void authorize() = 0;
    virtual void setNetworkAccessManager(QNetworkAccessManager* manager);
    
    void setAuthorizeWidget(AuthorizeWidget* widget);
    
protected:
    AuthorizeWidget* m_authWidget;
};

}

#endif // IBACKEND_H