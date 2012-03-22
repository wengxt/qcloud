#include "server.h"
#include "daemonadaptor.h"

namespace QCloud {
Server::Server(QObject* parent): QObject(parent)
    ,m_adaptor(new DaemonAdaptor(this))
{

}

Server::~Server()
{

}

}