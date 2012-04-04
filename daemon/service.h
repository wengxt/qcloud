#ifndef SERVICE_H
#define SERVICE_H

#include <QCoreApplication>
#include "server.h"

class QNetworkAccessManager;
class Service : public QCloud::Server
{
    Q_OBJECT
public:
    explicit Service (QObject* parent = 0);
    virtual ~Service();

    virtual QCloud::InfoList listApps();
    virtual QCloud::InfoList listBackends();
    virtual QCloud::InfoList listAccounts();
    virtual void addAccount (const QString& backendName, const QString& appName);
    virtual int uploadFile (const QString& app_name, const QStringList& file_list);
    virtual int sync (const QString& app_name);
    void setNetworkAccessManager(QNetworkAccessManager* networkAccessManager);

private:
    QNetworkAccessManager* m_networkAccessManager;

};


#endif // SERVICE_H