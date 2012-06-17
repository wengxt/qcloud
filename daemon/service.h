#ifndef SERVICE_H
#define SERVICE_H

#include <QCoreApplication>
#include "server.h"
#include "entryinfo.h"

class Daemon;
class QNetworkAccessManager;
class Service : public QCloud::Server
{
    Q_OBJECT
public:
    explicit Service (Daemon* daemon);
    virtual ~Service();

    virtual QCloud::InfoList listApps();
    virtual QCloud::InfoList listBackends();
    virtual QCloud::InfoList listAccounts();
    virtual void listFiles(const QString& uuid,const QString& directory);
    virtual void addAccount (const QString& backendName, const QString& appName);
    virtual void deleteAccount (const QString& uuid);
    virtual int uploadFile (const QString& app_name, const QStringList& file_list);
    virtual int sync (const QString& app_name);
    
public slots:
    virtual void listFilesRequestFinished();
    
private:
    Daemon* m_daemon;
    QCloud::EntryList entryList;
    QCloud::EntryInfo entryInfo;
};


#endif // SERVICE_H