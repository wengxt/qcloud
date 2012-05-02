#include "account.h"
#include "ibackend.h"

Account::Account (QCloud::IBackend* backend, const QUuid& uuid, QObject* parent) : QObject(parent)
    ,m_backend(backend)
    ,m_uuid(uuid)
{
    m_backend->setParent(this);
}

QCloud::App* Account::app()
{
    return m_backend->app();
}

QCloud::IBackend* Account::backend()
{
    return m_backend;
}

void Account::setUuid (const QUuid& uuid)
{
    m_uuid = uuid;
}

const QUuid& Account::uuid() const
{
    return m_uuid;
}

QString Account::userName()
{
    return m_backend->userName();
}
