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
        info.setIconName(account->backend()->info().iconName());
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
    if (!account)
        return -1;
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->pathInfo(directory,&requestHander->entryInfo,&requestHander->entryInfoList);
    requestHander->setRequest(request);
    return currentRequestId ++;
}

int Service::createFolder (const QString& uuid, const QString& directory)
{
    GeneralRequestHandler* requestHander = new GeneralRequestHandler(currentRequestId, this, this);
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->createFolder(directory);
    requestHander->setRequest(request);
    return currentRequestId ++;
}

int Service::deleteFile (const QString& uuid, const QString& path)
{
    GeneralRequestHandler* requestHander = new GeneralRequestHandler(currentRequestId, this, this);
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->deleteFile(path);
    requestHander->setRequest(request);
    return currentRequestId ++;
}

int Service::moveFile (const QString& uuid, const QString& src, const QString& dst)
{
    GeneralRequestHandler* requestHander = new GeneralRequestHandler(currentRequestId, this, this);
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->moveFile(src, dst);
    requestHander->setRequest(request);
    return currentRequestId ++;
}

int Service::copyFile (const QString& uuid, const QString& src, const QString& dst)
{
    GeneralRequestHandler* requestHander = new GeneralRequestHandler(currentRequestId, this, this);
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->copyFile(src, dst);
    requestHander->setRequest(request);
    return currentRequestId ++;
}

int Service::fetchInfo (const QString& uuid, const QString& path)
{
    FileInfoRequestHandler* requestHander = new FileInfoRequestHandler(currentRequestId, this, this);
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->pathInfo(path, &requestHander->entryInfo);
    requestHander->setRequest(request);
    return currentRequestId ++;
}


ListFilesRequestHandler::ListFilesRequestHandler(int id, QCloud::Server* server,QObject* parent)
{
    m_id = id;
    m_server = server;
}


void ListFilesRequestHandler::requestFinished()
{
    qDebug() << "Sending finished signal...";

    m_server->notifyDirectoryInfoTransformed(m_id, m_request->error(), entryInfoList);
    delete this;
}

ListFilesRequestHandler::~ListFilesRequestHandler()
{

}


FileInfoRequestHandler::FileInfoRequestHandler(int id, QCloud::Server* server,QObject* parent)
{
    m_id = id;
    m_server = server;
}


void FileInfoRequestHandler::requestFinished()
{
    qDebug() << "Sending finished signal...";

    m_server->notifyFileInfoTransformed(m_id, m_request->error(), entryInfo);
    delete this;
}

FileInfoRequestHandler::~FileInfoRequestHandler()
{

}


GeneralRequestHandler::GeneralRequestHandler(int id, QCloud::Server* server,QObject* parent)
{
    m_id = id;
    m_server = server;
}
GeneralRequestHandler::~GeneralRequestHandler()
{

}


void GeneralRequestHandler::requestFinished()
{
    qDebug() << "Sending finished signal...";

    m_server->notifyRequestFinished(m_id, m_request->error());
    delete this;
}
