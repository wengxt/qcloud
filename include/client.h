#ifndef QCLOUD_CLIENT_H
#define QCLOUD_CLIENT_H
#include <QtCore/QObject>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusPendingReply>
#include <QtCloud/App>
#include <QtCloud/Info>
#include <QtCloud/EntryInfo>
#include "qcloud_global.h"
#include "request.h"

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
    QDBusPendingReply<int> listFiles(const QString& uuid,const QString& directory);
    QDBusPendingReply<void> addAccount (const QString& backend_name, const QString& user_name);
    QDBusPendingReply<void> deleteAccount (const QString& uuid);
    QDBusPendingReply<int> sync (const QString& app_name);
    QDBusPendingReply<int> uploadFile (const QString& app_name, const QStringList& file_list);
    QDBusPendingReply<int> createFolder(const QString& uuid, const QString& path);
    QDBusPendingReply<int> deleteFile (const QString& uuid, const QString& path);
signals:
    void accountUpdated();
    void directoryInfoTransformed(int id, uint error, const QCloud::EntryInfoList &info);
    void requestFinished(int id, uint error);
protected:
    ClientPrivate* d;
};
}

#endif
