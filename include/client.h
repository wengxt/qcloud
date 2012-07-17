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
    QDBusPendingReply<int> uploadFile (const QString& uuid, const QString& file, uint type, const QString& dest);
    QDBusPendingReply<int> downloadFile (const QString& uuid, const QString& src, const QString& file, uint type);
    QDBusPendingReply<int> createFolder(const QString& uuid, const QString& path);
    QDBusPendingReply<int> deleteFile (const QString& uuid, const QString& path);
    QDBusPendingReply<int> moveFile (const QString& uid1, const QString& src, const QString& dest);
    QDBusPendingReply<int> copyFile (const QString& uid1, const QString& src, const QString& dest);
    QDBusPendingReply<int> fetchInfo (const QString& uuid, const QString& path);
signals:
    void resetRequest();
    void accountUpdated();
    void fileInfoTransformed(int id, uint error, const QCloud::EntryInfo &info);
    void directoryInfoTransformed(int id, uint error, const QCloud::EntryInfoList &info);
    void requestFinished(int id, uint error);
    void downloadProgress (int id, qlonglong sent, qlonglong total);
    void uploadProgress (int id, qlonglong sent, qlonglong total);
protected slots:
    void serviceOwnerChanged (const QString & serviceName, const QString & oldOwner, const QString & newOwner);
protected:
    ClientPrivate* d;
};
}

#endif
