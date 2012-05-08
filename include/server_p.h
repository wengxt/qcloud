#ifndef QCLOUD_SERVER_P_H
#define QCLOUD_SERVER_P_H

#include "server.h"

class DaemonAdaptor;

namespace QCloud
{
class ServerPrivate
{
public:
    ServerPrivate (Server* parent);
    virtual ~ServerPrivate();

    QDBusConnection session;
    DaemonAdaptor* adaptor;
    bool valid;
};
}

#endif //QCLOUD_SERVER_P_H