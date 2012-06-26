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
#include <client.h>

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
        kDebug() << "Stat root" << url;
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
    kDebug() << "CloudSlave::listDir" << url.url();
    KIO::SlaveBase::get (url);
}


void CloudSlave::listDir (const KUrl& url)
{
    kDebug() << "CloudSlave::listDir" << url.url();

    if (!m_client->isValid()) {
        kDebug() << "!!!!!!!!!!!!!!!NOT VALID";
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
            kDebug() << account.name() << account.iconName();
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
    else
        error(KIO::ERR_DOES_NOT_EXIST, url.prettyUrl());
}
