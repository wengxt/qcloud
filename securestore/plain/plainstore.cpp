#include <QtCore/QtPlugin>
#include <QtCore/QSettings>
#include "plainstore.h"

PlainStore::PlainStore (QObject* parent) : ISecureStore (parent)
{
    m_settings = new QSettings ("QCloud", "Secure Store");
}

PlainStore::~PlainStore()
{
    delete m_settings;
}

bool PlainStore::isAvailable()
{
    return true;
}

bool PlainStore::readItem (const QString& group, const QString& key, QByteArray& value)
{
    if (!containsItem (group, key))
        return false;

    m_settings->beginGroup (group);
    value = m_settings->value (key).toByteArray();
    m_settings->endGroup();
    return true;
}

bool PlainStore::writeItem (const QString& group, const QString& key, const QByteArray& value)
{
    if (!m_settings->isWritable())
        return false;

    m_settings->beginGroup (group);
    m_settings->setValue (key, value);
    m_settings->endGroup();
    return true;
}

bool PlainStore::deleteItem (const QString& group, const QString& key)
{
    if (!containsItem (group, key))
        return false;

    m_settings->beginGroup (group);
    m_settings->remove (key);
    m_settings->endGroup();
    return true;
}

bool PlainStore::containsItem (const QString& group, const QString& key)
{
    m_settings->beginGroup (group);
    bool isContain = m_settings->contains (key);
    m_settings->endGroup();
    return isContain;
}
