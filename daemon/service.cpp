#include "service.h"
#include "appmanager.h"
#include <factory.h>

Service::Service (QObject* parent) : Server (parent)
{

}

Service::~Service()
{

}

int Service::addAccount (const QString& backend_name, const QString& user_name, const QDBusVariant& account_specific_data)
{
    return 0;
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

