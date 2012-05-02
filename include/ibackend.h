#ifndef QCLOUD_IBACKEND_H
#define QCLOUD_IBACKEND_H

#include <QtCore/QtPlugin>
#include "qcloud_global.h"
#include "info.h"

class QNetworkAccessManager;
namespace QCloud
{

class ISecureStore;

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
    virtual void saveAccountInfo (const QString& key, QSettings& settings, ISecureStore* secureStore) = 0;
    virtual void loadAccountInfo (const QString& key, QSettings& settings, ISecureStore* secureStore) = 0;
    virtual void deleteSecretInfo (const QString& key, ISecureStore* securestore) = 0;
    virtual QString userName() = 0;
    QNetworkAccessManager* networkAccessManager();
    App* app();
    void setInfo(const Info& info);
    const Info& info();
protected:
    Info m_info;
    QNetworkAccessManager* m_networkAccessManager;
    App* m_app;
};

}

#endif // IBACKEND_H
