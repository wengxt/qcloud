#include "account.h"
#include "ibackend.h"

Account::Account (QCloud::IBackend* backend, QObject* parent) : QObject(parent)
    ,m_backend(backend)
    ,m_uuid(QUuid::createUuid())
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

const QUuid& Account::uuid()
{
    return m_uuid;
}

QString Account::userName()
{
    return m_backend->userName();
}
