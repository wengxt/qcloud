#include <QNetworkAccessManager>

#include "daemon.h"
#include "service.h"
#include "factory.h"

Daemon::Daemon (int& argc, char** argv) : QApplication (argc, argv)
    ,m_service(new Service(this))
{
    setQuitOnLastWindowClosed(false);
    m_networkAccessManager = QCloud::Factory::instance()->createNetwork("general");
    m_networkAccessManager->setParent(this);
    m_service->setNetworkAccessManager(m_networkAccessManager);
}

Daemon::~Daemon()
{

}
