#ifndef DAEMON_H
#define DAEMON_H

#include <QApplication>

class QNetworkAccessManager;
class Service;
class AccountManager;
namespace QCloud
{
class ISecureStore;
}

class Daemon : public QApplication
{
    Q_OBJECT
public:
    Daemon (int& argc, char** argv);
    virtual ~Daemon();
    virtual void saveState (QSessionManager& sm);
    QNetworkAccessManager* createNetwork();
    AccountManager* accountManager();
    QCloud::ISecureStore* secureStore();
private:
    QCloud::ISecureStore* m_secureStore;
    AccountManager* m_accountManager;
    Service* m_service;
    QString m_networkPluginName;
};

#endif // DAEMON_H