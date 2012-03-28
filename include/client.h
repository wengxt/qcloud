#ifndef QCLOUD_CLIENT_H
#define QCLOUD_CLIENT_H
#include <QtCore/QObject>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusPendingReply>

namespace QCloud {

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject* parent = 0);
    virtual ~Client();
    bool isValid();

    QDBusPendingReply<bool> addAccount(const QString &backend_name, const QString &user_name, const QDBusVariant &account_specific_data);
    QDBusPendingReply<int> sync(const QString &app_name);
    QDBusPendingReply<int> uploadFile(const QString &app_name, const QStringList &file_list);
protected:
    class Private;
    Private* d;
};
}

#endif