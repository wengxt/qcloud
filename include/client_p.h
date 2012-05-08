#include "client.h"

class OrgQcloudDaemonInterface;

namespace QCloud
{

class ClientPrivate
{
    friend class Client;
public:
    explicit ClientPrivate (Client* parent = 0);
    virtual ~ClientPrivate();

private:
    QDBusConnection connection;
    OrgQcloudDaemonInterface* daemon;
};
}
