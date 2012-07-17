#ifndef KIO_CLOUD_H
#define KIO_CLOUD_H

#define KIO_CLOUD_DEBUG

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
    QCloud::Client* client();

protected:
    virtual void listDir (const KUrl& url);
    virtual void stat (const KUrl& url);
    virtual void get (const KUrl& url);
    virtual void mkdir (const KUrl& url, int permissions);
    virtual void del (const KUrl& url, bool isfile);
    virtual void rename (const KUrl& src, const KUrl& dest, KIO::JobFlags flags);
    virtual void copy (const KUrl& src, const KUrl& dest, int permissions, KIO::JobFlags flags);
    virtual void put (const KUrl& url, int permissions, KIO::JobFlags flags);

    bool parseUrl (const KUrl& url, QString& uid, QString& path);
    QCloud::Client* m_client;
private:
};

QDebug & kioDebug();

#endif
