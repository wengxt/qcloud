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

class QCLOUD_EXPORT Client : public QObject
{
    Q_OBJECT
public:
    explicit Client (QObject* parent = 0);
    virtual ~Client();
    bool isValid();

    QDBusPendingReply<InfoList> listApps();
    QDBusPendingReply<InfoList> listBackends();
    QDBusPendingReply<bool> addAccount (const QString& backend_name, const QString& user_name);
    QDBusPendingReply<int> sync (const QString& app_name);
    QDBusPendingReply<int> uploadFile (const QString& app_name, const QStringList& file_list);
protected:
    class Private;
    Private* d;
};
}

#endif
