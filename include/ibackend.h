#ifndef QCLOUD_IBACKEND_H
#define QCLOUD_IBACKEND_H

#include <QtCore/QtPlugin>
#include "qcloud_global.h"

class QNetworkAccessManager;
namespace QCloud
{

class OAuthWidget;
class QCLOUD_EXPORT IBackend : public QObject
{
    Q_OBJECT
public:
    explicit IBackend (QObject* parent = 0);
    virtual ~IBackend();

    virtual bool authorize (QWidget* widget = 0) = 0;
    virtual void setNetworkAccessManager (QNetworkAccessManager* manager);
    virtual bool uploadFile (const QString& filename) = 0;
    QNetworkAccessManager* networkAccessManager();
protected:
    QNetworkAccessManager* m_networkAccessManager;
};

}

#endif // IBACKEND_H
