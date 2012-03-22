#include "server.h"
#include "daemonadaptor.h"

namespace QCloud {

Server::Server(QObject* parent): QObject(parent)
    ,m_session(QDBusConnection::sessionBus())
    ,m_adaptor(new DaemonAdaptor(this))
{
    m_session.registerObject("/daemon", this);
    m_session.registerService("org.qcloud.daemon");
}

Server::~Server()
{
    m_session.unregisterService("org.qcloud.daemon");
    m_session.unregisterObject("/daemon");
}


}