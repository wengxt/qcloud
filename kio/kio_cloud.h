#ifndef RECENTDOCUMENT_H
#define RECENTDOCUMENT_H

#include <KIO/ForwardingSlaveBase>

namespace QCloud
{
class Client;
}

class KDirWatch;

class CloudSlave : public KIO::SlaveBase
{
public:
    CloudSlave(const QByteArray &pool, const QByteArray &app);
    virtual ~CloudSlave();

protected:
    virtual void listDir (const KUrl& url);
    virtual void stat (const KUrl& url);
    virtual void get (const KUrl& url);
    virtual void mkdir (const KUrl& url, int permissions);
    virtual void del (const KUrl& url, bool isfile);
    QCloud::Client* m_client;
};

#endif
