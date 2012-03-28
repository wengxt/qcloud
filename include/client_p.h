#include "client.h"

class OrgQcloudDaemonInterface;

namespace QCloud {

class Client::Private : public QObject {
    friend class Client;
    Q_OBJECT
public:
    explicit Private (Client* parent = 0);
    virtual ~Private();

private:
    QDBusConnection m_connection;
    OrgQcloudDaemonInterface *m_daemon;
};
}