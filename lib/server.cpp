#include "server.h"
#include "daemonadaptor.h"

namespace QCloud
{

Server::Server (QObject* parent) : QObject (parent)
    , m_session (QDBusConnection::sessionBus())
    , m_adaptor (new DaemonAdaptor (this))
{
    Info::registerMetaType();
    m_session.registerObject ("/daemon", this);
    m_session.registerService ("org.qcloud.Daemon");
}

Server::~Server()
{
    m_session.unregisterService ("org.qcloud.Daemon");
    m_session.unregisterObject ("/daemon");
}

void Server::notifyAccountUpdated()
{
    emit accountUpdated();
}

}
