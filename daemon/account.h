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
    Account(QCloud::IBackend* backend, const QUuid& uuid = QUuid::createUuid(), QObject* parent = NULL);
    QCloud::IBackend* backend();
    QCloud::App* app();
    void setUuid(const QUuid& uuid);
    const QUuid& uuid() const;
    QString userName();
private:
    QCloud::IBackend* m_backend;
    QUuid m_uuid;
};


#endif // ACCOUNT_H