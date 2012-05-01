#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QUuid>
#include <QSettings>

namespace QCloud {
class IBackend;
class App;
}

class Account : public QObject {
    Q_OBJECT
public:
    Account(QCloud::IBackend* backend, QObject* parent);
    QCloud::IBackend* backend();
    QCloud::App* app();
    const QUuid& uuid();
    QString userName();
private:
    QCloud::IBackend* m_backend;
    QUuid m_uuid;
};


#endif // ACCOUNT_H