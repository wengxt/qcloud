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
    QNetworkAccessManager* networkAccessManager();
    AccountManager* accountManager();
    QCloud::ISecureStore* secureStore();
private:
    QNetworkAccessManager* m_networkAccessManager;
    QCloud::ISecureStore* m_secureStore;
    AccountManager* m_accountManager;
    Service* m_service;
};

#endif // DAEMON_H