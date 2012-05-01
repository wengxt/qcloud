#include "server.h"
#include "daemonadaptor.h"

namespace QCloud
{

Server::Server (QObject* parent) : QObject (parent)
    , m_session (QDBusConnection::sessionBus())
    , m_adaptor (new DaemonAdaptor (this))
    , m_valid (false)
{
    Info::registerMetaType();
    m_session.registerObject ("/daemon", this);
    if (m_session.registerService ("org.qcloud.Daemon"))
        m_valid = true;
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

bool Server::isValid() const
{
    return m_valid;
}

}
