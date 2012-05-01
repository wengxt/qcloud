#include "service.h"
#include "appmanager.h"
#include "factory.h"
#include "ibackend.h"
#include "daemon.h"
#include "accountmanager.h"
#include "account.h"

Service::Service (Daemon* daemon) : Server (daemon)
    ,m_daemon(daemon)
{

}

Service::~Service()
{

}

void Service::addAccount (const QString& backendName, const QString& appName)
{
    QCloud::App* app = QCloud::AppManager::instance()->app(appName);
    if (!app)
        return;
    QCloud::IBackend* backend = QCloud::Factory::instance()->createBackend(backendName);
    if (backend) {
        backend->setApp(app);
        backend->setNetworkAccessManager(m_daemon->createNetwork());
        if (backend->authorize()) {
            m_daemon->accountManager()->addAccount(backend);
            notifyAccountUpdated();
        }
        else
            delete backend;
    }
}

QCloud::InfoList Service::listApps()
{
    QCloud::InfoList list;
    QList<QCloud::App*> appList(QCloud::AppManager::instance()->appList());
    Q_FOREACH(QCloud::App* app, appList)
    {
        list << *app;
    }
    return list;
}

QCloud::InfoList Service::listBackends()
{
    QCloud::InfoList list;
    QList<QCloud::IPlugin*> backenList(QCloud::Factory::instance()->backendList());
    Q_FOREACH(QCloud::IPlugin* backend, backenList)
    {
        list << *backend;
    }
    return list;
}


int Service::sync (const QString& app_name)
{
    return 0;
}

int Service::uploadFile (const QString& app_name, const QStringList& file_list)
{
    return 0;
}

QCloud::InfoList Service::listAccounts()
{
    QCloud::InfoList infoList;
    QList< Account* > list = m_daemon->accountManager()->listAccounts();
    foreach(Account* account, list) {
        QCloud::Info info;
        info.setName(account->uuid());
        info.setDescription(QString("%1 - %2").arg(account->backend()->info().description()).arg(account->app()->description()));
        info.setIconName("user");
        info.setDisplayName(account->userName());

        infoList << info;
    }
    return infoList;
}
