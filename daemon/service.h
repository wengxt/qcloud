#ifndef SERVICE_H
#define SERVICE_H

#include <QCoreApplication>
#include "server.h"
#include "entryinfo.h"
#include "requesthandler.h"

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
    virtual int listFiles(const QString& uuid,const QString& directory);
    virtual void addAccount (const QString& backendName, const QString& appName);
    virtual void deleteAccount (const QString& uuid);
    virtual int uploadFile (const QString& app_name, const QStringList& file_list);
    virtual int sync (const QString& app_name);

private:
    Daemon* m_daemon;
    int currentRequestId;
};

class ListFilesRequestHandler : public QCloud::RequestHandler
{
    Q_OBJECT
    friend class Service;
public:
    explicit ListFilesRequestHandler(int id,QCloud::Server* server,QObject* parent=0);
    virtual ~ListFilesRequestHandler();

public slots:
    virtual void requestFinished();

protected:
    QCloud::EntryInfoList entryInfoList;
    QCloud::EntryInfo entryInfo;
};

#endif // SERVICE_H