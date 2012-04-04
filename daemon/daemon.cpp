#include "daemon.h"
#include "service.h"

Daemon::Daemon (int& argc, char** argv) : QCoreApplication (argc, argv)
    ,m_service(new Service(this))
{

}

Daemon::~Daemon()
{

}
