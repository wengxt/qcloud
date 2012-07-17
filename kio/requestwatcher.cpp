#include <QLocalSocket>
#include <QTimer>
#include <QDBusServiceWatcher>
#include "client.h"

#include "requestwatcher.h"
#include "kio_cloud.h"

RequestWatcherBase::RequestWatcherBase (int requestId, CloudSlave* slave, QObject* object) : QObject (object)
    ,m_requestId(requestId)
    ,m_slave(slave)
    ,m_error(0)
    ,m_finished(false)
{
    connect(m_slave->client(), SIGNAL(resetRequest()), SLOT(resetRequest()));
}

const QString& RequestWatcherBase::errorString()
{
    return m_errorString;
}

void RequestWatcherBase::resetRequest()
{
    m_error = QCloud::Request::NetworkError;
    disconnect(m_slave->client(), SIGNAL(resetRequest()), this, SLOT(resetRequest()));
    notifyFinished();
}

uint RequestWatcherBase::error()
{
    return m_error;
}

bool RequestWatcherBase::isFinished()
{
    return m_finished;
}

void RequestWatcherBase::notifyFinished()
{
    if (m_finished)
        return;
    m_finished = true;
    emit finished();
}


InfoRequestWatcher::InfoRequestWatcher (int requestId, CloudSlave* slave, QObject* object) : RequestWatcherBase(requestId, slave, object)
{
    connect(slave->client(), SIGNAL(fileInfoTransformed(int,uint,QCloud::EntryInfo)), this, SLOT(reveivedInfo(int,uint,QCloud::EntryInfo)));
}

void InfoRequestWatcher::reveivedInfo (int id, uint error, const QCloud::EntryInfo& info)
{
    if (id == m_requestId) {
        m_entryInfo = info;
        m_error = error;
        notifyFinished();
    }
}

const QCloud::EntryInfo& InfoRequestWatcher::entryInfo()
{
    return m_entryInfo;
}


InfoListRequestWatcher::InfoListRequestWatcher (int requestId, CloudSlave* slave, QObject* object) : RequestWatcherBase(requestId, slave, object)
{
    connect(slave->client(), SIGNAL(directoryInfoTransformed(int,uint,QCloud::EntryInfoList)), this, SLOT(reveivedInfoList(int,uint,QCloud::EntryInfoList)));
}

const QCloud::EntryInfoList& InfoListRequestWatcher::entryInfoList()
{
    return m_entryInfoList;
}

void InfoListRequestWatcher::reveivedInfoList (int id, uint error, const QCloud::EntryInfoList& info)
{
    if (id == m_requestId) {
        m_entryInfoList = info;
        m_error = error;
        notifyFinished();
    }
}


GeneralRequestWatcher::GeneralRequestWatcher (int requestId, CloudSlave* slave, QObject* object) : RequestWatcherBase(requestId, slave, object)
{
    connect(slave->client(), SIGNAL(requestFinished(int,uint)), SLOT(requestFinished(int,uint)));
}

void GeneralRequestWatcher::requestFinished (int id, uint error)
{
    if (id == m_requestId) {
        m_error = error;
        notifyFinished();
    }
}

Uploader::Uploader (int requestId, QLocalServer* server, CloudSlave* slave, QObject* object) : GeneralRequestWatcher (requestId, slave, object)
    ,m_server(server)
    ,m_socket(0)
{
    if (m_server->hasPendingConnections()) {
        onNewConnection();
    }
    else
        connect(m_server, SIGNAL(newConnection()), SLOT(onNewConnection()));
}

Uploader::~Uploader()
{
    if (m_socket)
        delete m_socket;
}


void Uploader::onNewConnection()
{
    m_socket = m_server->nextPendingConnection();
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), SLOT(onError(QLocalSocket::LocalSocketError)));
    QTimer::singleShot(0, this, SLOT(run()));
}

void Uploader::onError (QLocalSocket::LocalSocketError error)
{
    // kioDebug() << "Uploader::onError" << error;
    m_error = QCloud::Request::NetworkError;
    notifyFinished();
}


void Uploader::run()
{
    QByteArray array;
    int result;
    do {
        m_slave->dataReq();
        result = m_slave->readData(array);
        if (result > 0) {
            result = m_socket->write(array);
            // kioDebug() << "Uploader::run write result: " << result;
        }
    } while (result > 0);

    if (result < 0)
        m_error = QCloud::Request::NetworkError;
    else {
        m_socket->waitForBytesWritten();
        m_socket->disconnectFromServer();
    }
}

Downloader::Downloader (int requestId, QLocalServer* server, CloudSlave* slave, QObject* object) : GeneralRequestWatcher (requestId, slave, object)
    ,m_server(server)
    ,m_socket(0)
{
    if (m_server->hasPendingConnections()) {
        onNewConnection();
    }
    else
        connect(m_server, SIGNAL(newConnection()), SLOT(onNewConnection()));
}

Downloader::~Downloader()
{
    if (m_socket)
        delete m_socket;
}


void Downloader::onNewConnection()
{
    m_socket = m_server->nextPendingConnection();
    m_server->close();
    connect(m_socket, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), SLOT(onError(QLocalSocket::LocalSocketError)));
    connect(m_slave->client(), SIGNAL(downloadProgress(int,qlonglong,qlonglong)), SLOT(downloadProgress(int,qlonglong,qlonglong)));
    run();
}

void Downloader::downloadProgress (int id, qlonglong sent, qlonglong total)
{
    if (id != m_requestId)
        return;

    // kioDebug() << "Downloader::downloadProgress" << id << sent << total;

    if (total > 0)
        m_slave->totalSize(total);
    m_slave->processedSize(sent);
}


void Downloader::onError (QLocalSocket::LocalSocketError error)
{
    if (error != QLocalSocket::PeerClosedError) {
        m_error = QCloud::Request::NetworkError;
    }
    notifyFinished();
}

void Downloader::run()
{
    char buf[8192];
    int result = 0;
    do {
        result = m_socket->read(buf, sizeof(buf));
        if (result > 0) {
            m_slave->data(QByteArray(buf, result));
        }
        // kioDebug() << "Downloader::run" << result;
    } while(result > 0);

    if (result < 0) {
        m_error = QCloud::Request::NetworkError;
        m_errorString = m_socket->errorString();
        notifyFinished();
    }
}

void Downloader::readyRead()
{
    run();
}

void Downloader::requestFinished (int id, uint error)
{
    // kioDebug() << "Downloader::requestFinished" << m_error;
    int result = 0;
    if (m_socket->bytesAvailable()) {
        char buf[8192];
        do {
            result = m_socket->read(buf, sizeof(buf));
            if (result > 0)
                m_slave->data(QByteArray(buf, result));
            // kioDebug() << "Downloader::run" << result;
        } while(result > 0);
    }
    m_slave->data(QByteArray());

    if (result < 0) {
        m_error = QCloud::Request::NetworkError;
        m_errorString = m_socket->errorString();
    }
    GeneralRequestWatcher::requestFinished (id, error);
}
