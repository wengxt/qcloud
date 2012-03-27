#include <QtCore/QtPlugin>
#include "plainstore.h"

PlainStore::PlainStore()
{
}


PlainStore::~PlainStore()
{

}

bool PlainStore::GetItem(const QString& key, QString& value)
{
    return false;
}

bool PlainStore::SetItem(const QString& key, const QString& value)
{
    return false;
}

bool PlainStore::isAvailable()
{
    return true;
}
