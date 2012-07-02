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

void Server::notifyDirectoryInfoTransformed(const QCloud::EntryInfoList& info,int id)
{
    emit directoryInfoTransformed(info,id);
    qDebug() << "signal directoryInfoTransformed() sent";
}

bool Server::isValid() const
{
    return d->valid;
}

}
