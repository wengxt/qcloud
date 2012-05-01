#include "accountmanager.h"
#include "account.h"
#include "daemon.h"
#include "app.h"
#include "ibackend.h"
#include "factory.h"
#include "appmanager.h"

AccountManager::AccountManager(Daemon* daemon) : QObject(daemon)
    ,m_daemon(daemon)
    ,m_settings("qcloud", "account")
{
}

void AccountManager::loadAccounts()
{
    m_settings.beginGroup("account");
    QStringList uuidList = m_settings.childGroups();
    foreach(const QString& uuid, uuidList) {
        Account* account;
        if (m_accounts.count(uuid) > 0) {
            account = m_accounts[uuid];
        }
        else {
            account = parseConfig(uuid);
            if (account)
                m_accounts[uuid] = account;
        }
    }
    m_settings.endGroup();
}

void AccountManager::addAccount(QCloud::IBackend* backend)
{
    Account* account = new Account(backend, this);
    m_settings.beginGroup("account");
    m_settings.beginGroup(account->uuid().toString());
    m_settings.setValue("App", account->app()->name());
    m_settings.setValue("Backend", account->backend()->info().name());
    account->backend()->saveAccountInfo(account->uuid(), m_settings, m_daemon->secureStore());
    m_settings.endGroup();
    m_settings.endGroup();
    m_accounts[account->uuid()] = account;
}

void AccountManager::deleteAccount (const QUuid& uuid)
{

}

Account* AccountManager::parseConfig (const QString& id)
{
    m_settings.beginGroup(id);
    QUuid uuid(id);
    QString appName = m_settings.value("App").toString();
    QString backendName= m_settings.value("Backend").toString();

    Account* account = NULL;
    do {
        QCloud::App* app = QCloud::AppManager::instance()->app(appName);
        if (!app)
            break;
        QCloud::IBackend* backend = QCloud::Factory::instance()->createBackend(backendName);
        if (!backend)
            break;
        backend->setApp(app);
        backend->setNetworkAccessManager(m_daemon->createNetwork());
        backend->loadAccountInfo(uuid.toString(), m_settings, m_daemon->secureStore());
        account = new Account(backend, this);
    } while(0);
    m_settings.endGroup();
    return account;
}

Account* AccountManager::findAccount (const QUuid& uuid)
{
    return NULL;
}

QList< Account* > AccountManager::listAccounts()
{
    return m_accounts.values();
}
