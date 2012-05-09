#ifndef SERVICE_H
#define SERVICE_H

#include <QCoreApplication>
#include "server.h"

class Daemon;
class QNetworkAccessManager;
class Service : public QCloud::Server
{
    Q_OBJECT
public:
    explicit Service (Daemon* daemon);
    virtual ~Service();

    virtual QCloud::InfoList listApps();
    virtual QCloud::InfoList listBackends();
    virtual QCloud::InfoList listAccounts();
    virtual QCloud::InfoList listFiles(const QString& uuid);
    virtual void addAccount (const QString& backendName, const QString& appName);
    virtual void deleteAccount (const QString& uuid);
    virtual int uploadFile (const QString& app_name, const QStringList& file_list);
    virtual int sync (const QString& app_name);

private:
    Daemon* m_daemon;
};


#endif // SERVICE_H