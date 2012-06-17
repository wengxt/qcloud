#include "client.h"
#include "client_p.h"
#include "org.qcloud.Daemon.h"

namespace QCloud
{

ClientPrivate::ClientPrivate (Client* client) :
    connection (QDBusConnection::sessionBus())
    , daemon (new org::qcloud::Daemon ("org.qcloud.Daemon", "/daemon", connection, client))
{
}


ClientPrivate::~ClientPrivate()
{
}

Client::Client (QObject* parent) : QObject (parent)
    , d (new ClientPrivate (this))
{
    connect(d->daemon, SIGNAL(accountUpdated()), this, SIGNAL(accountUpdated()));
    connect(d->daemon, SIGNAL(directoryInfoTransformed(QCloud::InfoList,int)), this, SIGNAL(directoryInfoTransformed(QCloud::InfoList,int)));
}

Client::~Client()
{
    delete d;
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

QDBusPendingReply< int > Client::uploadFile (const QString& app_name, const QStringList& file_list)
{
    return d->daemon->uploadFile (app_name, file_list);
}

}
