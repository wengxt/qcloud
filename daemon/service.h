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
    virtual int uploadFile (const QString& uuid, const QString& file, uint type, const QString& dest);
    virtual int downloadFile (const QString& uuid, const QString& src, const QString& file, uint type);
    virtual int sync (const QString& app_name);
    virtual int createFolder (const QString& uuid, const QString& directory);
    virtual int deleteFile (const QString& uuid, const QString& path);
    virtual int moveFile (const QString& uuid, const QString& src, const QString& dst);
    virtual int copyFile (const QString& uuid, const QString& src, const QString& dst);
    virtual int fetchInfo (const QString& uuid, const QString& directory);

private:
    Daemon* m_daemon;
    int currentRequestId;
};

class ListFilesRequestHandler : public QCloud::RequestHandler
{
    Q_OBJECT
    friend class Service;
public:
    explicit ListFilesRequestHandler(int id, QCloud::Server* server);
    virtual ~ListFilesRequestHandler();

public slots:
    virtual void requestFinished();

protected:
    QCloud::EntryInfoList entryInfoList;
    QCloud::EntryInfo entryInfo;
};

class FileInfoRequestHandler : public QCloud::RequestHandler
{
    Q_OBJECT
    friend class Service;
public:
    explicit FileInfoRequestHandler(int id, QCloud::Server* server );
    virtual ~FileInfoRequestHandler();

public slots:
    virtual void requestFinished();

protected:
    QCloud::EntryInfo entryInfo;
};

class GeneralRequestHandler : public QCloud::RequestHandler
{
    Q_OBJECT
    friend class Service;
public:
    explicit GeneralRequestHandler(int id, QCloud::Server* server);
    virtual ~GeneralRequestHandler();

public slots:
    virtual void requestFinished();
};


#endif // SERVICE_H