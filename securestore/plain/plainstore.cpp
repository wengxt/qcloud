#include <QtCore/QtPlugin>
#include "plainstore.h"

namespace QCloud {
    
PlainStore::PlainStore()
{
    setObjectName("plain");
}


PlainStore::~PlainStore()
{

}

QString PlainStore::GetItem(const QString& key)
{
    return QString();
}

bool PlainStore::SetItem(const QString& key, const QString& value)
{
    return false;
}

bool PlainStore::isAvailable()
{
    return true;
}


}

Q_EXPORT_PLUGIN2(securestore_plain, QCloud::PlainStore)