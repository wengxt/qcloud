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

int Service::uploadFile (const QString& uuid, const QString& file, uint type, const QString& dest)
{
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    GeneralRequestHandler* requestHandler = new GeneralRequestHandler(currentRequestId, this);
    QCloud::Request* request = account->backend()->uploadFile(file, type, dest);
    requestHandler->setRequest(request);
    return currentRequestId ++;
}


int Service::downloadFile (const QString& uuid, const QString& src, const QString& file, uint type)
{
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    GeneralRequestHandler* requestHandler = new GeneralRequestHandler(currentRequestId, this);
    QCloud::Request* request = account->backend()->downloadFile(src, file, type);
    requestHandler->setRequest(request);
    return currentRequestId ++;
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
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    ListFilesRequestHandler* requestHandler = new ListFilesRequestHandler(currentRequestId, this);
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->pathInfo(directory,&requestHandler->entryInfo,&requestHandler->entryInfoList);
    requestHandler->setRequest(request);
    return currentRequestId ++;
}

int Service::createFolder (const QString& uuid, const QString& directory)
{
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    GeneralRequestHandler* requestHandler = new GeneralRequestHandler(currentRequestId, this);
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->createFolder(directory);
    requestHandler->setRequest(request);
    return currentRequestId ++;
}

int Service::deleteFile (const QString& uuid, const QString& path)
{
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    GeneralRequestHandler* requestHandler = new GeneralRequestHandler(currentRequestId, this);
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->deleteFile(path);
    requestHandler->setRequest(request);
    return currentRequestId ++;
}

int Service::moveFile (const QString& uuid, const QString& src, const QString& dst)
{
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    GeneralRequestHandler* requestHandler = new GeneralRequestHandler(currentRequestId, this);
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->moveFile(src, dst);
    requestHandler->setRequest(request);
    return currentRequestId ++;
}

int Service::copyFile (const QString& uuid, const QString& src, const QString& dst)
{
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    GeneralRequestHandler* requestHandler = new GeneralRequestHandler(currentRequestId, this);
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->copyFile(src, dst);
    requestHandler->setRequest(request);
    return currentRequestId ++;
}

int Service::fetchInfo (const QString& uuid, const QString& path)
{
    Account *account = m_daemon->accountManager()->findAccount(uuid);
    if (!account)
        return -1;
    FileInfoRequestHandler* requestHandler = new FileInfoRequestHandler(currentRequestId, this);
    qDebug() << account->backend()->userName();
    QCloud::Request* request = account->backend()->pathInfo(path, &requestHandler->entryInfo);
    requestHandler->setRequest(request);
    return currentRequestId ++;
}

int Service::queryProgress (int request_id)
{
    return 0;
}



ListFilesRequestHandler::ListFilesRequestHandler(int id, QCloud::Server* server) : RequestHandler(server)
{
    m_id = id;
    m_server = server;
}


void ListFilesRequestHandler::requestFinished()
{
    qDebug() << "Sending finished signal..." << m_id << m_request->metaObject()->className();

    m_server->notifyDirectoryInfoTransformed(m_id, m_request->error(), entryInfoList);
    delete this;
}

ListFilesRequestHandler::~ListFilesRequestHandler()
{

}


FileInfoRequestHandler::FileInfoRequestHandler(int id, QCloud::Server* server) : RequestHandler(server)
{
    m_id = id;
    m_server = server;
}


void FileInfoRequestHandler::requestFinished()
{
    qDebug() << "Sending finished signal..." << m_id << m_request->metaObject()->className();

    m_server->notifyFileInfoTransformed(m_id, m_request->error(), entryInfo);
    delete this;
}

FileInfoRequestHandler::~FileInfoRequestHandler()
{

}


GeneralRequestHandler::GeneralRequestHandler(int id, QCloud::Server* server) : RequestHandler(server)
{
    m_id = id;
    m_server = server;
}
GeneralRequestHandler::~GeneralRequestHandler()
{

}


void GeneralRequestHandler::requestFinished()
{
    qDebug() << "Sending finished signal..." << m_id << m_request->metaObject()->className();

    m_server->notifyRequestFinished(m_id, m_request->error());
    delete this;
}
