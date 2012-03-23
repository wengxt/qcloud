#include "client.h"
#include "client_p.h"
#include "org.qcloud.daemon.h"

namespace QCloud {

Client::Private::Private(Client* client) : QObject(client)
    ,m_connection(QDBusConnection::sessionBus())
    ,m_daemon(new org::qcloud::Daemon("org.qcloud.Daemon", "/daemon", m_connection, this))
{
}


Client::Private::~Private()
{
}

Client::Client(QObject* parent) : QObject(parent)
    ,d(new Private(this))
{

}

Client::~Client()
{

}

bool Client::isValid()
{
    return d->m_daemon->isValid();
}

QDBusPendingReply< bool > Client::addAccount (const QString& backend_name, const QString& user_name, const QDBusVariant& account_specific_data)
{
    return d->m_daemon->addAccount(backend_name, user_name, account_specific_data);
}

QDBusPendingReply< int > Client::sync (const QString& app_name)
{
    return d->m_daemon->sync(app_name);
}

QDBusPendingReply< int > Client::uploadFile (const QString& app_name, const QStringList& file_list)
{
    return d->m_daemon->uploadFile(app_name, file_list);
}

}