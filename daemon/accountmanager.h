#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H
#include <QSettings>
#include <QUuid>
#include <QHash>

class Account;
class Daemon;
namespace QCloud
{
class IBackend;
}

class AccountManager : public QObject {
    Q_OBJECT
public:
    AccountManager(Daemon* daemon);

    void loadAccounts();
    void addAccount(QCloud::IBackend* backend);
    Account* findAccount(const QUuid& uuid);
    bool deleteAccount(const QUuid& uuid);
    QList<Account*> listAccounts();
private:
    Account* parseConfig(const QString& id);

    Daemon* m_daemon;
    QSettings m_settings;
    QHash<QUuid, Account*> m_accounts;
};

#endif