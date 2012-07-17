#include "server.h"
#include "server_p.h"
#include "daemonadaptor.h"

namespace QCloud
{
ServerPrivate::ServerPrivate (Server* server) :
    session(QDBusConnection::sessionBus())
    ,adaptor(new DaemonAdaptor(server))
    ,valid(false)
{

}

ServerPrivate::~ServerPrivate()
{
}

Server::Server (QObject* parent) : QObject (parent)
    , d(new ServerPrivate(this))
{
    Info::registerMetaType();
    EntryInfo::registerMetaType();
    d->session.registerObject ("/daemon", this);
    if (d->session.registerService ("org.qcloud.Daemon"))
        d->valid = true;
}

Server::~Server()
{
    d->session.unregisterService ("org.qcloud.Daemon");
    d->session.unregisterObject ("/daemon");
    delete d;
}

void Server::notifyAccountUpdated()
{
    emit accountUpdated();
}

void Server::notifyDirectoryInfoTransformed(int id, QCloud::Request::Error error, const QCloud::EntryInfoList& info)
{
    emit directoryInfoTransformed(id,(uint)error,info);
    qDebug() << "signal directoryInfoTransformed() sent";
}

void Server::notifyFileInfoTransformed(int id, QCloud::Request::Error error, const QCloud::EntryInfo& info)
{
    emit fileInfoTransformed(id,(uint)error,info);
    qDebug() << "signal fileInfoTransformed() sent";
}

void Server::notifyRequestFinished(int id, QCloud::Request::Error error)
{
    emit requestFinished(id, (uint)error);
    qDebug() << "signal Request Finished sent"; 
}

bool Server::isValid() const
{
    return d->valid;
}

}
