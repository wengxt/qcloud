#ifndef REQUESTWATCHER_H
#define REQUESTWATCHER_H
#include <QObject>
#include <QLocalSocket>
#include <QLocalServer>
#include "entryinfo.h"

class QLocalSocket;
class CloudSlave;
class RequestWatcherBase : public QObject {
    Q_OBJECT
public:
    RequestWatcherBase(int requestId, CloudSlave* slave, QObject* object = 0);
    uint error();
    const QString& errorString();
    bool isFinished();
signals:
    void finished();

protected slots:
    void resetRequest();

protected:
    void notifyFinished();
    int m_requestId;
    CloudSlave* m_slave;
    uint m_error;
    QString m_errorString;
    bool m_finished;
};

class GeneralRequestWatcher : public RequestWatcherBase {
    Q_OBJECT
public:
    GeneralRequestWatcher (int requestId, CloudSlave* slave, QObject* object = 0);

protected slots:
    virtual void requestFinished (int id, uint error);
};

class InfoRequestWatcher : public RequestWatcherBase {
    Q_OBJECT
public:
    InfoRequestWatcher (int requestId, CloudSlave* slave, QObject* object = 0);
    const QCloud::EntryInfo& entryInfo();

protected slots:
    void reveivedInfo (int id, uint error, const QCloud::EntryInfo& info);

private:
    QCloud::EntryInfo m_entryInfo;
};

class InfoListRequestWatcher : public RequestWatcherBase {
    Q_OBJECT
public:
    InfoListRequestWatcher (int requestId, CloudSlave* slave, QObject* object = 0);
    const QCloud::EntryInfoList& entryInfoList();

protected slots:
    void reveivedInfoList (int id, uint error, const QCloud::EntryInfoList& infoList);

private:
    QCloud::EntryInfoList m_entryInfoList;
};

class Uploader : public GeneralRequestWatcher {
    Q_OBJECT
public:
    Uploader(int requestId, QLocalServer* server, CloudSlave* slave, QObject* object = 0);
    virtual ~Uploader();
protected slots:
    void onError(QLocalSocket::LocalSocketError error);
    void onNewConnection();
    void run();
protected:
    QLocalServer* m_server;
    QLocalSocket* m_socket;
};

class Downloader : public GeneralRequestWatcher {
    Q_OBJECT
public:
    Downloader(int requestId, QLocalServer* server, CloudSlave* slave, QObject* object = 0);
    virtual ~Downloader();
protected slots:
    void onError(QLocalSocket::LocalSocketError error);
    void readyRead ();
    void onNewConnection();
    virtual void requestFinished (int id, uint error);
    void downloadProgress (int id, qlonglong sent, qlonglong total);
protected:
    void run();
    QLocalServer* m_server;
    QLocalSocket* m_socket;
};

#endif