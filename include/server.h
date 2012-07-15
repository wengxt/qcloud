#ifndef QCLOUD_SERVER_H
#define QCLOUD_SERVER_H
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusVariant>
#include <QtCloud/App>
#include <QtCloud/Info>
#include "qcloud_global.h"
#include "entryinfo.h"
#include "request.h"

namespace QCloud
{

class ServerPrivate;

class QCLOUD_EXPORT Server : public QObject
{
    Q_OBJECT
public:
    explicit Server (QObject* parent = 0);
    virtual ~Server();
    virtual InfoList listApps() = 0;
    virtual InfoList listBackends() = 0;
    virtual InfoList listAccounts() = 0;
    virtual int listFiles(const QString& uuid,const QString& directory) = 0;
    virtual int uploadFile (const QString& uuid, const QString& file, uint type, const QString& dest) = 0;
    virtual int downloadFile (const QString& uuid, const QString& src, const QString& file, uint type) = 0;
    virtual void addAccount (const QString& backend_name, const QString& user_name) = 0;
    virtual void deleteAccount (const QString& uuid) = 0;
    virtual int sync (const QString& app_name) = 0;
    virtual int createFolder (const QString& uuid, const QString& directory) = 0;
    virtual int deleteFile (const QString& uuid, const QString& path) = 0;
    virtual int moveFile (const QString& uuid, const QString& src, const QString& dst) = 0;
    virtual int copyFile (const QString& uuid, const QString& src, const QString& dst) = 0;
    virtual int fetchInfo (const QString& uuid, const QString& directory) = 0;
    virtual int queryProgress (int request_id) = 0;
    void notifyAccountUpdated();
    void notifyDirectoryInfoTransformed(int id, QCloud::Request::Error error, const QCloud::EntryInfoList& info);
    void notifyFileInfoTransformed(int id, QCloud::Request::Error error, const QCloud::EntryInfo& info);
    void notifyRequestFinished(int id, QCloud::Request::Error error);

    bool isValid() const;

signals:
    void accountUpdated();
    void requestFinished(int id, uint error);
    void directoryInfoTransformed(int id, uint error, const QCloud::EntryInfoList& info);
    void fileInfoTransformed(int id, uint error, const QCloud::EntryInfo& info);
private:
    Q_DISABLE_COPY (Server)
    ServerPrivate* d;
};
}

#endif
