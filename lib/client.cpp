#include "client.h"
#include "client_p.h"
#include "org.qcloud.Daemon.h"

namespace QCloud
{

ClientPrivate::ClientPrivate (Client* client) :
    connection (QDBusConnection::sessionBus())
    , daemon (new org::qcloud::Daemon ("org.qcloud.Daemon", "/daemon", connection, client))
    , watcher (new QDBusServiceWatcher(client))
{
}


ClientPrivate::~ClientPrivate()
{
}

Client::Client (QObject* parent) : QObject (parent)
    , d (new ClientPrivate (this))
{
    d->watcher->addWatchedService(d->daemon->service());

    connect(d->watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)), this, SLOT(serviceOwnerChanged(QString,QString,QString)));
    connect(d->daemon, SIGNAL(accountUpdated()), this, SIGNAL(accountUpdated()));
    connect(d->daemon, SIGNAL(directoryInfoTransformed(int,uint,QCloud::EntryInfoList)), this, SIGNAL(directoryInfoTransformed(int,uint,QCloud::EntryInfoList)));
    connect(d->daemon, SIGNAL(requestFinished(int,uint)), this, SIGNAL(requestFinished(int,uint)));
    connect(d->daemon, SIGNAL(fileInfoTransformed(int,uint,QCloud::EntryInfo)), this, SIGNAL(fileInfoTransformed(int,uint,QCloud::EntryInfo)));
    connect(d->daemon, SIGNAL(downloadProgress(int,qlonglong,qlonglong)), this, SIGNAL(downloadProgress(int,qlonglong,qlonglong)));
    connect(d->daemon, SIGNAL(uploadProgress(int,qlonglong,qlonglong)), this, SIGNAL(uploadProgress(int,qlonglong,qlonglong)));
}

Client::~Client()
{
    delete d;
}

void Client::serviceOwnerChanged(const QString & serviceName, const QString & oldOwner, const QString & newOwner)
{
    if (!newOwner.isEmpty()) {
        emit resetRequest();
    }
}

bool Client::isValid()
{
    return d->daemon->isValid();
}

QDBusPendingReply<InfoList> Client::listApps()
{
    return d->daemon->listApps();
}

QDBusPendingReply<InfoList> Client::listBackends()
{
    return d->daemon->listBackends();
}

QDBusPendingReply<InfoList> Client::listAccounts()
{
    return d->daemon->listAccounts();
}

QDBusPendingReply<int> Client::listFiles(const QString& uuid,const QString& directory)
{
    return d->daemon->listFiles(uuid,directory);
}

QDBusPendingReply< void > Client::addAccount (const QString& backend_name, const QString& user_name)
{
    return d->daemon->addAccount (backend_name, user_name);
}

QDBusPendingReply< void > Client::deleteAccount (const QString& uuid)
{
    return d->daemon->deleteAccount(uuid);
}

QDBusPendingReply< int > Client::sync (const QString& app_name)
{
    return d->daemon->sync (app_name);
}

QDBusPendingReply< int > Client::uploadFile (const QString &uuid, const QString &file, uint type, const QString &dest)
{
    return d->daemon->uploadFile (uuid, file, type, dest);
}

QDBusPendingReply< int > Client::downloadFile (const QString &uuid, const QString &src, const QString &file, uint type)
{
    return d->daemon->downloadFile (uuid, src, file, type);
}

QDBusPendingReply<int> Client::createFolder(const QString& uuid,const QString& directory)
{
    return d->daemon->createFolder(uuid, directory);
}

QDBusPendingReply<int> Client::deleteFile(const QString& uuid,const QString& directory)
{
    return d->daemon->deleteFile(uuid, directory);
}

QDBusPendingReply<int> Client::moveFile(const QString& uuid,const QString& src, const QString& dest)
{
    return d->daemon->moveFile(uuid, src, dest);
}

QDBusPendingReply<int> Client::copyFile(const QString& uuid,const QString& src, const QString& dest)
{
    return d->daemon->copyFile(uuid, src, dest);
}

QDBusPendingReply<int> Client::fetchInfo(const QString& uuid,const QString& directory)
{
    return d->daemon->fetchInfo(uuid, directory);
}


}
