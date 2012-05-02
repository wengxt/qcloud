#ifndef QCLOUD_SERVER_H
#define QCLOUD_SERVER_H
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusVariant>
#include <QtCloud/App>
#include <QtCloud/Info>
#include "qcloud_global.h"

class DaemonAdaptor;
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
    virtual int uploadFile (const QString& app_name, const QStringList& file_list) = 0;
    virtual void addAccount (const QString& backend_name, const QString& user_name) = 0;
    virtual void deleteAccount (const QString& uuid) = 0;
    virtual int sync (const QString& app_name) = 0;
    void notifyAccountUpdated();
    bool isValid() const;
signals:
    void accountUpdated();
private:
    Q_DISABLE_COPY (Server)
    QDBusConnection m_session;
    DaemonAdaptor* m_adaptor;
    bool m_valid;
};
}

#endif
