#include "service.h"
#include "appmanager.h"
#include "factory.h"
#include "ibackend.h"

Service::Service (QObject* parent) : Server (parent)
    ,m_networkAccessManager(0)
{

}

void Service::setNetworkAccessManager (QNetworkAccessManager* networkAccessManager)
{
    m_networkAccessManager = networkAccessManager;
}


Service::~Service()
{

}

void Service::addAccount (const QString& backendName, const QString& appName)
{
    QCloud::IBackend* backend = QCloud::Factory::instance()->createBackend(backendName);
    QCloud::App* app = QCloud::AppManager::instance()->app(appName);
    if (backend && app) {
        backend->setApp(app);
        backend->setNetworkAccessManager(m_networkAccessManager);
        backend->authorize();
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
    return QCloud::InfoList();
}
