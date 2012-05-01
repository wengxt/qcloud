#include <QNetworkAccessManager>

#include "daemon.h"
#include "service.h"
#include "accountmanager.h"
#include "factory.h"

Daemon::Daemon (int& argc, char** argv) : QApplication (argc, argv)
    ,m_networkAccessManager(QCloud::Factory::instance()->createNetwork("general", this))
    ,m_secureStore(QCloud::Factory::instance()->createSecureStore("kwallet", this))
    ,m_accountManager(new AccountManager(this))
    ,m_service(new Service(this))
{
    setQuitOnLastWindowClosed(false);
    m_accountManager->loadAccounts();
    m_service->notifyAccountUpdated();
}

Daemon::~Daemon()
{

}

QNetworkAccessManager* Daemon::networkAccessManager()
{
    return m_networkAccessManager;
}

AccountManager* Daemon::accountManager()
{
    return m_accountManager;
}

QCloud::ISecureStore* Daemon::secureStore()
{
    return m_secureStore;
}
