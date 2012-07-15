#include <QNetworkAccessManager>
#include <QTimer>
#include <QSessionManager>

#include "daemon.h"
#include "service.h"
#include "accountmanager.h"
#include "factory.h"

Daemon::Daemon (int& argc, char** argv) : QApplication (argc, argv)
    ,m_secureStore(QCloud::Factory::instance()->createSecureStore("kwallet", this))
    ,m_accountManager(new AccountManager(this))
    ,m_service(new Service(this))
    ,m_networkPluginName("kde")
{
    if (!m_service->isValid()) {
        QTimer::singleShot(0, this, SLOT(quit()));
        return;
    }
    setQuitOnLastWindowClosed(false);
    m_accountManager->loadAccounts();
    m_service->notifyAccountUpdated();
}

Daemon::~Daemon()
{

}

void Daemon::saveState (QSessionManager& sm)
{
    sm.setRestartHint(QSessionManager::RestartNever);
}


QNetworkAccessManager* Daemon::createNetwork()
{
    return QCloud::Factory::instance()->createNetwork(m_networkPluginName);
}

AccountManager* Daemon::accountManager()
{
    return m_accountManager;
}

QCloud::ISecureStore* Daemon::secureStore()
{
    return m_secureStore;
}
