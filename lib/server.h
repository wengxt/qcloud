#ifndef QCLOUD_SERVER_H
#define QCLOUD_SERVER_H
#include <QObject>

class DaemonAdaptor;
namespace QCloud {
class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(QObject* parent = 0);
    virtual ~Server();
private:
    DaemonAdaptor* m_adaptor;
};
}

#endif