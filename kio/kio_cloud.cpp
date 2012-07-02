#include <QCoreApplication>
#include <QDBusInterface>
#include <QFileInfo>

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
#include <client.h>


#define KIO_CLOUD_DEBUG

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
        RequestWatcher watcher(requestId);
        QObject::connect(m_client, SIGNAL(fileInfoTransformed(int,uint,QCloud::EntryInfo)), &watcher, SLOT(reveivedInfo(int,uint,QCloud::EntryInfo)));

        QEventLoop loop;
        QObject::connect(&watcher, SIGNAL(end()), &loop, SLOT(quit()));
        loop.exec();
        kioDebug() << "Stat root" << url;
        //
        // stat the root path
        //
        KIO::UDSEntry uds;
        kioDebug() << watcher.m_entryinfo.path() << watcher.m_entryinfo.modifiedTime() << watcher.m_entryinfo.isDir();
        QFileInfo f(watcher.m_entryinfo.path());
        uds.insert(KIO::UDSEntry::UDS_NAME, f.fileName());
        uds.insert(KIO::UDSEntry::UDS_SIZE, watcher.m_entryinfo.size());
        uds.insert(KIO::UDSEntry::UDS_ICON_NAME, watcher.m_entryinfo.icon());
        uds.insert(KIO::UDSEntry::UDS_FILE_TYPE, watcher.m_entryinfo.isDir()? S_IFDIR: S_IFREG);
        uds.insert(KIO::UDSEntry::UDS_ACCESS, watcher.m_entryinfo.isDir() ? (S_IRUSR | S_IWUSR | S_IXUSR) : (S_IRUSR | S_IWUSR));
        uds.insert(KIO::UDSEntry::UDS_MODIFICATION_TIME, watcher.m_entryinfo.modifiedTime().toTime_t());
        uds.insert(KIO::UDSEntry::UDS_MIME_TYPE, watcher.m_entryinfo.mime());

        statEntry(uds);
        finished();
    }
}

void CloudSlave::get (const KUrl& url)
{
    kioDebug() << "CloudSlave::listDir" << url.url();
    KIO::SlaveBase::get (url);
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
        QString path = url.path();
        if (!path.isEmpty() && path[0] == '/')
            path = path.mid(1);
        QString uid = path.section('/', 0, 0);
        kioDebug() <<  path << uid << path.section('/', 1);
        QDBusPendingReply< int > requestId = m_client->listFiles(uid, path.section('/', 1));
        requestId.waitForFinished();

        kioDebug() << uid << path.section('/', 1);

        if (requestId.isError()) {
            error(KIO::ERR_COULD_NOT_CONNECT, url.prettyUrl());
            return;
        }

        int id = requestId.value();
        if (id < 0) {
            error(KIO::ERR_UNSUPPORTED_ACTION, url.prettyUrl());
            return;
        }

        RequestWatcher watcher(requestId);
        QObject::connect(m_client, SIGNAL(directoryInfoTransformed(int,uint,QCloud::EntryInfoList)), &watcher, SLOT(receivedList(int,uint,QCloud::EntryInfoList)));

        QEventLoop loop;
        QObject::connect(&watcher, SIGNAL(end()), &loop, SLOT(quit()));
        loop.exec();

        KIO::UDSEntry uds;
        Q_FOREACH(const QCloud::EntryInfo & item, watcher.m_entryinfolist) {
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
        totalSize(watcher.m_entryinfolist.count());
        listEntry(uds, true);

        finished();
    }
}

void CloudSlave::mkdir (const KUrl& url, int permissions)
{
    QString path = url.path();
    if (!path.isEmpty() && path[0] == '/')
        path = path.mid(1);
    QString uid = path.section('/', 0, 0);
    kioDebug() <<  path << uid << path.section('/', 1);
    QDBusPendingReply< int > requestId = m_client->createFolder(uid, path.section('/', 1));
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
    RequestWatcher watcher(requestId);
    QObject::connect(m_client, SIGNAL(requestFinished(int,uint)), &watcher, SLOT(requestFinished(int,uint)));

    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(end()), &loop, SLOT(quit()));
    loop.exec();
    finished();
}

void CloudSlave::del (const KUrl& url, bool isfile)
{
    QString path = url.path();
    if (!path.isEmpty() && path[0] == '/')
        path = path.mid(1);
    QString uid = path.section('/', 0, 0);
    kioDebug() <<  path << uid << path.section('/', 1);
    QDBusPendingReply< int > requestId = m_client->deleteFile(uid, path.section('/', 1));
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
    RequestWatcher watcher(requestId);
    QObject::connect(m_client, SIGNAL(requestFinished(int,uint)), &watcher, SLOT(requestFinished(int,uint)));

    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(end()), &loop, SLOT(quit()));
    loop.exec();
    finished();
}

void CloudSlave::rename (const KUrl& src, const KUrl& dest, KIO::JobFlags flags)
{
    QString path1 = src.path();
    QString path2 = dest.path();
    if (!path1.isEmpty() && path1[0] == '/')
        path1 = path1.mid(1);
    if (!path2.isEmpty() && path2[0] == '/')
        path2 = path2.mid(1);
    QString uid1 = path1.section('/', 0, 0);
    QString uid2 = path2.section('/', 0, 0);
    if (uid1 != uid2) {
        error(KIO::ERR_UNSUPPORTED_ACTION, "move file cannot across account");
        return;
    }

    kioDebug() <<  path1 << uid1 << path1.section('/', 1);
    kioDebug() <<  path2 << uid2 << path2.section('/', 1);
    QDBusPendingReply< int > requestId = m_client->moveFile(uid1, path1.section('/', 1), path2.section('/', 1));
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
    RequestWatcher watcher(requestId);
    QObject::connect(m_client, SIGNAL(requestFinished(int,uint)), &watcher, SLOT(requestFinished(int,uint)));

    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(end()), &loop, SLOT(quit()));
    loop.exec();
    finished();
}

void CloudSlave::copy (const KUrl& src, const KUrl& dest, int permissions, KIO::JobFlags flags)
{
    QString path1 = src.path();
    QString path2 = dest.path();
    if (!path1.isEmpty() && path1[0] == '/')
        path1 = path1.mid(1);
    if (!path2.isEmpty() && path2[0] == '/')
        path2 = path2.mid(1);
    QString uid1 = path1.section('/', 0, 0);
    QString uid2 = path2.section('/', 0, 0);
    if (uid1 != uid2) {
        error(KIO::ERR_UNSUPPORTED_ACTION, "move file cannot across account");
        return;
    }

    kioDebug() <<  path1 << uid1 << path1.section('/', 1);
    kioDebug() <<  path2 << uid2 << path2.section('/', 1);
    QDBusPendingReply< int > requestId = m_client->copyFile(uid1, path1.section('/', 1), path2.section('/', 1));
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
    RequestWatcher watcher(requestId);
    QObject::connect(m_client, SIGNAL(requestFinished(int,uint)), &watcher, SLOT(requestFinished(int,uint)));

    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(end()), &loop, SLOT(quit()));
    loop.exec();
    switch (watcher.m_error) {
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
