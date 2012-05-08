#ifndef QCLOUD_CLIENT_H
#define QCLOUD_CLIENT_H
#include <QtCore/QObject>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusPendingReply>
#include <QtCloud/App>
#include <QtCloud/Info>
#include "qcloud_global.h"

namespace QCloud
{

class ClientPrivate;
class QCLOUD_EXPORT Client : public QObject
{
    Q_OBJECT
public:
    explicit Client (QObject* parent = 0);
    virtual ~Client();
    bool isValid();

    QDBusPendingReply<InfoList> listApps();
    QDBusPendingReply<InfoList> listBackends();
    QDBusPendingReply<InfoList> listAccounts();
    QDBusPendingReply<void> addAccount (const QString& backend_name, const QString& user_name);
    QDBusPendingReply<void> deleteAccount (const QString& uuid);
    QDBusPendingReply<int> sync (const QString& app_name);
    QDBusPendingReply<int> uploadFile (const QString& app_name, const QStringList& file_list);
signals:
    void accountUpdated();
protected:
    ClientPrivate* d;
};
}

#endif
