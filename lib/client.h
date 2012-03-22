#ifndef QCLOUD_CLIENT_H
#define QCLOUD_CLIENT_H
#include <QObject>

namespace QCloud {
class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject* parent = 0);
    virtual ~Client();
};
}

#endif