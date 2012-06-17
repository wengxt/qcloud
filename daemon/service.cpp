#include "service.h"
#include "appmanager.h"
#include "factory.h"
#include "ibackend.h"
#include "daemon.h"
#include "accountmanager.h"
#include "account.h"
#include "request.h"
#include "entryinfo.h"
#include <QFileInfo>

#include <QDebug>

Service::Service (Daemon* daemon) : Server (daemon)
    ,m_daemon(daemon)
{
    currentRequestId = 0;
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

void Service::deleteAccount(const QString& strid)
{
    QUuid uuid(strid);
    qDebug() << "Ask accountManager to delete uuid : " << uuid;
    if (m_daemon->accountManager()->deleteAccount(uuid)) {
        qDebug() << uuid << " deleted";
        notifyAccountUpdated();
    }
}

QCloud::InfoList Service::listApps()
{
    QCloud::InfoList list;
    QList<QCloud::App*> appList(QCloud::AppManager::instance()->appList());
    foreach(QCloud::App* app, appList)
    {
        list << *app;
    }
    return list;
}

QCloud::InfoList Service::listBackends()
{
    QCloud::InfoList list;
    QList<QCloud::IPlugin*> backenList(QCloud::Factory::instance()->backendList());
    foreach(QCloud::IPlugin* backend, backenList)
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
        qDebug() << "UUID : " << account->uuid() << "userName : " << account->userName();
    }
    return infoList;
}

int Service::listFiles(const QString& uuid,const QString& directory)
{
    ListFilesRequestHandler* requestHander = new ListFilesRequestHandler(currentRequestId,this,this);
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->pathInfo(directory,&requestHander->entryInfo,&requestHander->entryList);
    requestHander->setRequest(request);
    currentRequestId ++;
    return currentRequestId - 1;
}

ListFilesRequestHandler::ListFilesRequestHandler(int id,QCloud::Server* server,QObject* parent)
{
    m_id = id;
    m_server = server;
}


void ListFilesRequestHandler::requestFinished()
{
    qDebug() << "Sending finished signal...";
    QCloud::InfoList infoList;
    infoList.clear();
    QCloud::Info info;
    //set ..
    QFileInfo fileInfo(entryInfo.path());
    info.setName(fileInfo.path());
    if (entryInfo.isDir())
        info.setDescription("is_dir");
    else
        info.setDescription("not_a_dir");
    info.setDisplayName("..");
    info.setIconName(entryInfo.icon());
    infoList << info;
    
    foreach(QCloud::EntryInfo i,entryList){
        fileInfo = QFileInfo(i.path());
        info.setName(i.path());
        if (i.isDir())
            info.setDescription("is_dir");
        else
            info.setDescription("not_a_dir");
        info.setDisplayName(fileInfo.fileName());
        info.setIconName(i.icon());
        
        qDebug() << info.name() << " " << info.description();
        
        infoList << info;
    }
    
    m_server->notifyDirectoryInfoTransformed(infoList,m_id);   
    delete this;
}

ListFilesRequestHandler::~ListFilesRequestHandler()
{
    
}
