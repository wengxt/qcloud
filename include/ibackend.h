#ifndef QCLOUD_IBACKEND_H
#define QCLOUD_IBACKEND_H

#include <QtCore/QtPlugin>
#include "qcloud_global.h"

class QNetworkAccessManager;
namespace QCloud
{

class Request;

class App;

class QCLOUD_EXPORT IBackend : public QObject
{
    Q_OBJECT
public:
    explicit IBackend (QObject* parent = 0);
    virtual ~IBackend();

    virtual void setApp (App* app) = 0;
    virtual bool authorize (QWidget* widget = 0) = 0;
    virtual void setNetworkAccessManager (QNetworkAccessManager* manager);
    virtual Request* uploadFile (const QString& localFileName, const QString& remoteFilePath) = 0;
    virtual Request* downloadFile(const QString& remoteFilePath,const QString& localFileName) = 0;
    virtual void saveAccountInfo () = 0;
    virtual void loadAccountInfo () = 0;
    QNetworkAccessManager* networkAccessManager();
protected:
    QNetworkAccessManager* m_networkAccessManager;
    App* m_app;
};

}

#endif // IBACKEND_H
