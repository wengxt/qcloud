#include "accountmanager.h"
#include "account.h"
#include "daemon.h"
#include "app.h"
#include "ibackend.h"
#include "factory.h"
#include "appmanager.h"

#include <QDebug>

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
        if (m_accounts.contains(uuid)) {
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
    Account* account = new Account(backend, QUuid::createUuid(), this);
    m_settings.beginGroup("account");
    m_settings.beginGroup(account->uuid().toString());
    m_settings.setValue("App", account->app()->name());
    m_settings.setValue("Backend", account->backend()->info().name());
    account->backend()->saveAccountInfo(account->uuid(), m_settings, m_daemon->secureStore());
    m_settings.endGroup();
    m_settings.endGroup();
    m_accounts[account->uuid()] = account;
    qDebug() << account->uuid();
}

bool AccountManager::deleteAccount (const QUuid& uuid)
{
    qDebug() << "accout number before delete: " << m_accounts.count() << m_accounts.keys();
    if (!m_accounts.contains(uuid))
        return false;
    Account* accounts = m_accounts[uuid];
    m_settings.beginGroup("account");
    m_settings.remove(uuid.toString());
    m_settings.endGroup();
    qDebug() << m_settings.allKeys();
    accounts->backend()->deleteSecretInfo(uuid.toString(), m_daemon->secureStore());
    delete accounts;
    int ret = m_accounts.remove(uuid);
    if (ret != 0) {
        qDebug() << "accout number: " << m_accounts.count();
        qDebug() << "manager successfully deleted : " << uuid.toString();
        return true;
    }
    return false;
}

Account* AccountManager::parseConfig (const QString& id)
{
    m_settings.beginGroup(id);
    QUuid uuid(id);
    if (uuid.isNull())
        return NULL;
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
        account = new Account(backend, uuid, this);
    } while(0);
    m_settings.endGroup();
    return account;
}

Account* AccountManager::findAccount (const QUuid& uuid)
{
    return m_accounts[uuid];
}

QList< Account* > AccountManager::listAccounts()
{
    return m_accounts.values();
}
