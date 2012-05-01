#include <QtCore/QtPlugin>
#include "plainstore.h"

PlainStore::PlainStore(QObject* parent) : ISecureStore(parent)
{
}


PlainStore::~PlainStore()
{

}

bool PlainStore::readItem (const QString& group, const QString& key, QByteArray& value)
{
    return false;
}

bool PlainStore::writeItem (const QString& group, const QString& key, const QByteArray& value)
{
    return false;
}

bool PlainStore::deleteItem (const QString& group, const QString& key)
{
    return false;
}


bool PlainStore::isAvailable()
{
    return true;
}
