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
        uds.insert(KIO::UDSEntry::UDS_ACCESS, 0500);
        uds.insert(KIO::UDSEntry::UDS_MIME_TYPE, QString::fromLatin1("inode/directory"));

        statEntry(uds);
        finished();
    }
    // results are forwarded
    else {
        error(KIO::ERR_NO_CONTENT, "blablabla");
        return;
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
            uds.insert(KIO::UDSEntry::UDS_ACCESS, 0500);
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
        QObject::connect(m_client, SIGNAL(directoryInfoTransformed(QCloud::EntryInfoList,int)), &watcher, SLOT(receivedList(QCloud::EntryInfoList,int)));

        QEventLoop loop;
        QObject::connect(&watcher, SIGNAL(end()), &loop, SLOT(quit()));
        loop.exec();

        KIO::UDSEntry uds;
        Q_FOREACH(const QCloud::EntryInfo & item, watcher.m_entryinfo) {
            kioDebug() << item.path() << item.modifiedTime() << item.isDir();
            QFileInfo f(item.path());
            uds.clear();
            uds.insert(KIO::UDSEntry::UDS_NAME, f.fileName());
            uds.insert(KIO::UDSEntry::UDS_SIZE, item.size());
            uds.insert(KIO::UDSEntry::UDS_ICON_NAME, item.icon());
            uds.insert(KIO::UDSEntry::UDS_FILE_TYPE, item.isDir()? S_IFDIR: S_IFREG);
            uds.insert(KIO::UDSEntry::UDS_ACCESS, 0500);
            uds.insert(KIO::UDSEntry::UDS_MODIFICATION_TIME, item.modifiedTime().toTime_t());
            uds.insert(KIO::UDSEntry::UDS_MIME_TYPE, item.mime());
            listEntry(uds, false);
        }
        totalSize(watcher.m_entryinfo.count());
        listEntry(uds, true);

        finished();
    }
}
