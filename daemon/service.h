#ifndef SERVICE_H
#define SERVICE_H

#include <QCoreApplication>
#include "server.h"

class Service : public QCloud::Server
{
    Q_OBJECT
public:
    explicit Service (QObject* parent = 0);
    virtual ~Service();

    virtual QCloud::InfoList listApps();
    virtual QCloud::InfoList listBackends();
    virtual int addAccount (const QString& backend_name, const QString& user_name, const QDBusVariant& account_specific_data);
    virtual int uploadFile (const QString& app_name, const QStringList& file_list);
    virtual int sync (const QString& app_name);
};


#endif // SERVICE_H