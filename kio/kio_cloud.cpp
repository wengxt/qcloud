#include <QCoreApplication>
#include <QDBusInterface>
#include <QFileInfo>
#include <QUuid>
#include <QLocalServer>
#include <QLocalSocket>

#include <KDebug>
#include <KComponentData>
#include <KRecentDocument>
#include <KDirWatch>
#include <KDesktopFile>
#include <KStandardDirs>
#include <KIO/Job>
#include <KIO/NetAccess>

#include <stdio.h>

#include "kio_cloud.h"
#include "requestwatcher.h"
#include "client.h"
#include "ibackend.h"

#ifdef KIO_CLOUD_DEBUG
class KioDebug {
public:
    QDebug & kioDebug() {
        static QFile log_file("/tmp/kio_cloud");
        log_file.flush();
        log_file.close();
        log_file.open(QIODevice::WriteOnly | QIODevice::Append);

        static QDebug log_stream(&log_file);
        return log_stream;
    }
} _kiodebug;

QDebug & kioDebug()
{
    return _kiodebug.kioDebug() << endl;
}
#else
QDebug kioDebug()
{
    return qDebug();
}
#endif

extern "C" int KDE_EXPORT kdemain(int argc, char **argv)
{
    // necessary to use other kio slaves
    QCoreApplication app(argc, argv);
    KComponentData("kio_cloud", "qcloud");
    KGlobal::locale();
    if (argc != 4) {
        fprintf(stderr, "Usage: kio_cloud protocol domain-socket1 domain-socket2\n");
        exit(-1);
    }
    QCloud::Info::registerMetaType();
    QCloud::EntryInfo::registerMetaType();
    // start the slave
    CloudSlave slave(argv[2], argv[3]);
    slave.dispatchLoop();
    return 0;
}

bool isRootUrl(const KUrl& url)
{
    const QString path = url.path(KUrl::RemoveTrailingSlash);
    return(!url.hasQuery() &&
           (path.isEmpty() || path == QLatin1String("/")));
}

CloudSlave::CloudSlave(const QByteArray& pool, const QByteArray& app):
    SlaveBase("cloud", pool, app),
    m_client(new QCloud::Client)
{
}

CloudSlave::~CloudSlave()
{
}

QCloud::Client* CloudSlave::client()
{
    return m_client;
}


void CloudSlave::stat (const KUrl& url)
{
    if (isRootUrl(url)) {
        kioDebug() << "Stat root" << url;
        //
        // stat the root path
        //
        KIO::UDSEntry uds;
        uds.insert(KIO::UDSEntry::UDS_NAME, i18n("Cloud service"));
        uds.insert(KIO::UDSEntry::UDS_DISPLAY_NAME, i18n("Cloud service"));
        uds.insert(KIO::UDSEntry::UDS_ICON_NAME, QString::fromLatin1("qcloud"));
        uds.insert(KIO::UDSEntry::UDS_FILE_TYPE, S_IFDIR);
        uds.insert(KIO::UDSEntry::UDS_ACCESS, S_IRUSR | S_IXUSR);
        uds.insert(KIO::UDSEntry::UDS_MIME_TYPE, QString::fromLatin1("inode/directory"));

        statEntry(uds);
        finished();
    }
    // results are forwarded
    else {
        QString path = url.path();
        if (!path.isEmpty() && path[0] == '/')
            path = path.mid(1);
        QString uid = path.section('/', 0, 0);
        kioDebug() <<  path << uid << path.section('/', 1);
        QDBusPendingReply< int > requestId = m_client->fetchInfo(uid, path.section('/', 1));
        requestId.waitForFinished();

        if (requestId.isError()) {
            error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
            return;
        }

        int id = requestId.value();
        if (id < 0) {
            error(KIO::ERR_UNSUPPORTED_ACTION, url.prettyUrl());
            return;
        }
        InfoRequestWatcher watcher(requestId, this);

        QEventLoop loop;
        QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        kioDebug() << "Stat root" << url;
        //
        // stat the root path
        //
        KIO::UDSEntry uds;
        kioDebug() << watcher.entryInfo().path() << watcher.entryInfo().modifiedTime() << watcher.entryInfo().isDir();
        QFileInfo f(watcher.entryInfo().path());
        uds.insert(KIO::UDSEntry::UDS_NAME, f.fileName());
        uds.insert(KIO::UDSEntry::UDS_SIZE, watcher.entryInfo().size());
        uds.insert(KIO::UDSEntry::UDS_ICON_NAME, watcher.entryInfo().icon());
        uds.insert(KIO::UDSEntry::UDS_FILE_TYPE, watcher.entryInfo().isDir()? S_IFDIR: S_IFREG);
        uds.insert(KIO::UDSEntry::UDS_ACCESS, watcher.entryInfo().isDir() ? (S_IRUSR | S_IWUSR | S_IXUSR) : (S_IRUSR | S_IWUSR));
        uds.insert(KIO::UDSEntry::UDS_MODIFICATION_TIME, watcher.entryInfo().modifiedTime().toTime_t());
        uds.insert(KIO::UDSEntry::UDS_MIME_TYPE, watcher.entryInfo().mime());

        statEntry(uds);
        finished();
    }
}

bool CloudSlave::parseUrl(const KUrl& url, QString& uuid, QString &path)
{
    QString fullpath = url.path();
    if (!fullpath.isEmpty() && fullpath[0] == '/')
        fullpath = fullpath.mid(1);
    uuid = fullpath.section('/', 0, 0);
    path = fullpath.section('/', 1);
    return true;
}

void CloudSlave::listDir (const KUrl& url)
{
    kioDebug() << "CloudSlave::listDir" << url.url();

    if (!m_client->isValid()) {
        kioDebug() << "!!!!!!!!!!!!!!!NOT VALID";
        error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
    }

    if (isRootUrl(url)) {
        QDBusPendingReply< QCloud::InfoList > result = m_client->listAccounts();
        result.waitForFinished();

        if (result.isError()) {
            error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
            return;
        }

        KIO::UDSEntry uds;
        Q_FOREACH(const QCloud::Info & account, result.value()) {
            kioDebug() << account.name() << account.iconName();
            uds.clear();
            uds.insert(KIO::UDSEntry::UDS_NAME, account.name());
            uds.insert(KIO::UDSEntry::UDS_DISPLAY_NAME, account.displayName());
            uds.insert(KIO::UDSEntry::UDS_ICON_NAME, account.iconName());
            uds.insert(KIO::UDSEntry::UDS_FILE_TYPE, S_IFDIR);
            uds.insert(KIO::UDSEntry::UDS_ACCESS, S_IRUSR | S_IWUSR | S_IXUSR);
            uds.insert(KIO::UDSEntry::UDS_MIME_TYPE, QString::fromLatin1("inode/directory"));
            listEntry(uds, false);
        }
        totalSize(result.value().count());
        listEntry(uds, true);

        finished();
    }
    else {
        QString path, uid;
        parseUrl(url, uid, path);
        kioDebug() << uid << path;
        QDBusPendingReply< int > requestId = m_client->listFiles(uid, path);
        requestId.waitForFinished();

        if (requestId.isError()) {
            error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
            return;
        }

        int id = requestId.value();
        if (id < 0) {
            error(KIO::ERR_UNSUPPORTED_ACTION, url.prettyUrl());
            return;
        }

        InfoListRequestWatcher watcher(requestId, this);
        QEventLoop loop;
        QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        KIO::UDSEntry uds;
        Q_FOREACH(const QCloud::EntryInfo & item, watcher.entryInfoList()) {
            kioDebug() << item.path() << item.modifiedTime() << item.isDir();
            QFileInfo f(item.path());
            uds.clear();
            uds.insert(KIO::UDSEntry::UDS_NAME, f.fileName());
            uds.insert(KIO::UDSEntry::UDS_SIZE, item.size());
            uds.insert(KIO::UDSEntry::UDS_ICON_NAME, item.icon());
            uds.insert(KIO::UDSEntry::UDS_FILE_TYPE, item.isDir()? S_IFDIR: S_IFREG);
            uds.insert(KIO::UDSEntry::UDS_ACCESS, item.isDir() ? (S_IRUSR | S_IWUSR | S_IXUSR) : (S_IRUSR | S_IWUSR));
            uds.insert(KIO::UDSEntry::UDS_MODIFICATION_TIME, item.modifiedTime().toTime_t());
            uds.insert(KIO::UDSEntry::UDS_MIME_TYPE, item.mime());
            listEntry(uds, false);
        }
        totalSize(watcher.entryInfoList().count());
        listEntry(uds, true);

        finished();
    }
}

void CloudSlave::mkdir (const KUrl& url, int permissions)
{
    QString path, uid;
    parseUrl(url, uid, path);
    kioDebug() << uid << path;
    QDBusPendingReply< int > requestId = m_client->createFolder(uid, path);
    requestId.waitForFinished();

    if (requestId.isError()) {
        error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
        return;
    }

    int id = requestId.value();
    if (id < 0) {
        error(KIO::ERR_UNSUPPORTED_ACTION, url.prettyUrl());
        return;
    }
    GeneralRequestWatcher watcher(requestId, this);
    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    finished();
}

void CloudSlave::del (const KUrl& url, bool isfile)
{
    QString path, uid;
    parseUrl(url, uid, path);
    kioDebug() << uid << path;
    QDBusPendingReply< int > requestId = m_client->deleteFile(uid, path);
    requestId.waitForFinished();

    if (requestId.isError()) {
        error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
        return;
    }

    int id = requestId.value();
    if (id < 0) {
        error(KIO::ERR_UNSUPPORTED_ACTION, url.prettyUrl());
        return;
    }
    GeneralRequestWatcher watcher(requestId, this);
    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    finished();
}

void CloudSlave::rename (const KUrl& src, const KUrl& dest, KIO::JobFlags flags)
{
    QString path1, uid1, path2, uid2;
    parseUrl(src, uid1, path1);
    parseUrl(src, uid2, path2);
    kioDebug() << uid1 << path1;
    kioDebug() << uid2 << path2;
    if (uid1 != uid2) {
        error(KIO::ERR_UNSUPPORTED_ACTION, "move file cannot across account");
        return;
    }
    QDBusPendingReply< int > requestId = m_client->moveFile(uid1, path1, path2);
    requestId.waitForFinished();

    if (requestId.isError()) {
        error(KIO::ERR_COULD_NOT_CONNECT, src.prettyUrl());
        return;
    }

    int id = requestId.value();
    if (id < 0) {
        error(KIO::ERR_UNSUPPORTED_ACTION, src.prettyUrl());
        return;
    }
    GeneralRequestWatcher watcher(requestId, this);
    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    finished();
}

void CloudSlave::copy (const KUrl& src, const KUrl& dest, int permissions, KIO::JobFlags flags)
{
    QString path1, uid1, path2, uid2;
    parseUrl(src, uid1, path1);
    parseUrl(src, uid2, path2);
    kioDebug() << uid1 << path1;
    kioDebug() << uid2 << path2;
    if (uid1 != uid2) {
        error(KIO::ERR_UNSUPPORTED_ACTION, "move file cannot across account");
        return;
    }

    QDBusPendingReply< int > requestId = m_client->copyFile(uid1, path1, path2);
    requestId.waitForFinished();

    if (requestId.isError()) {
        error(KIO::ERR_COULD_NOT_CONNECT, src.prettyUrl());
        return;
    }

    int id = requestId.value();
    if (id < 0) {
        error(KIO::ERR_UNSUPPORTED_ACTION, src.prettyUrl());
        return;
    }
    GeneralRequestWatcher watcher(requestId, this);
    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    switch (watcher.error()) {
        case QCloud::Request::NoError:
            finished();
            break;
        case QCloud::Request::FileExistsError:
            error(KIO::ERR_FILE_ALREADY_EXIST, dest.prettyUrl());
            break;
        case QCloud::Request::FileNotFoundError:
            error(KIO::ERR_DOES_NOT_EXIST, dest.prettyUrl());
            break;
        default:
            error(KIO::ERR_UNKNOWN, dest.prettyUrl());
            break;
    }
}


void CloudSlave::put (const KUrl& url, int permissions, KIO::JobFlags flags)
{
    if (!m_client->isValid()) {
        kioDebug() << "!!!!!!!!!!!!!!!NOT VALID";
        error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
    }

    QString path, uid;
    parseUrl(url, uid, path);
    kioDebug() << uid << path;

    QLocalServer server;
    server.setMaxPendingConnections(1);
    QUuid uuid = QUuid::createUuid();
    QString serverName = QString("/tmp/qcloud-%1").arg(uuid.toString());
    if (!server.listen(serverName)) {
        kioDebug() << "Not able to start the Server";
    }
    QDBusPendingReply< int > resultRequest = m_client->uploadFile(uid, server.serverName(), QCloud::IBackend::LocalSocket, path);
    resultRequest.waitForFinished();
    int requestId = resultRequest.value();
    Uploader watcher(requestId, &server, this);

    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QFile file(serverName);
    file.remove();

    if (watcher.error()) {
        error(KIO::ERR_WRITE_ACCESS_DENIED, url.prettyUrl());
    }
    else {
        finished();
    }
}

void CloudSlave::get (const KUrl& url)
{
    if (!m_client->isValid()) {
        kioDebug() << "!!!!!!!!!!!!!!!NOT VALID";
        error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
    }

    QString path, uid;
    parseUrl(url, uid, path);
    kioDebug() << uid << path;

    QLocalServer server;
    server.setMaxPendingConnections(1);
    QUuid uuid = QUuid::createUuid();
    QString serverName = QString("/tmp/qcloud-%1").arg(uuid.toString());
    if (!server.listen(serverName)) {
        kioDebug() << "Not able to start the Server";
    }
    QDBusPendingReply< int > resultRequest = m_client->downloadFile(uid, path, server.serverName(), QCloud::IBackend::LocalSocket);
    resultRequest.waitForFinished();
    int requestId = resultRequest.value();
    Downloader watcher(requestId, &server, this);

    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QFile file(serverName);
    file.remove();

    if (watcher.error()) {
        kioDebug() << "ERROR" << watcher.error() << watcher.errorString();
        error(KIO::ERR_UNKNOWN, watcher.errorString());
    }
    else {
        finished();
    }
}
