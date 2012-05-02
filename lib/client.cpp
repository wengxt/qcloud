#include "client.h"
#include "client_p.h"
#include "org.qcloud.Daemon.h"

namespace QCloud
{

Client::Private::Private (Client* client) : QObject (client)
    , m_connection (QDBusConnection::sessionBus())
    , m_daemon (new org::qcloud::Daemon ("org.qcloud.Daemon", "/daemon", m_connection, this))
{
    connect(m_daemon, SIGNAL(accountUpdated()), client, SIGNAL(accountUpdated()));
}


Client::Private::~Private()
{
}

Client::Client (QObject* parent) : QObject (parent)
    , d (new Private (this))
{

}

Client::~Client()
{

}

bool Client::isValid()
{
    return d->m_daemon->isValid();
}

QDBusPendingReply<InfoList> Client::listApps()
{
    return d->m_daemon->listApps();
}

QDBusPendingReply<InfoList> Client::listBackends()
{
    return d->m_daemon->listBackends();
}

QDBusPendingReply<InfoList> Client::listAccounts()
{
    return d->m_daemon->listAccounts();
}

QDBusPendingReply< bool > Client::addAccount (const QString& backend_name, const QString& user_name)
{
    return d->m_daemon->addAccount (backend_name, user_name);
}

QDBusPendingReply< bool > Client::deleteAccount (const QString& uuid)
{
    return d->m_daemon->deleteAccount(uuid);
}

QDBusPendingReply< int > Client::sync (const QString& app_name)
{
    return d->m_daemon->sync (app_name);
}

QDBusPendingReply< int > Client::uploadFile (const QString& app_name, const QStringList& file_list)
{
    return d->m_daemon->uploadFile (app_name, file_list);
}

}
