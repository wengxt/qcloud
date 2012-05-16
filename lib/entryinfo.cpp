#include <QDebug>
#include "entryinfo.h"
#include "request.h"

namespace QCloud{
    
EntryInfo::~EntryInfo()
{
    
}

EntryInfo::EntryInfo()
{
    
}

qulonglong EntryInfo::size()
{
    return m_size;
}

bool EntryInfo::isDir()
{
    return is_dir;
}

QDateTime EntryInfo::modifiedTime()
{
    return m_modifiedTime;
}

QString EntryInfo::hash()
{
    return m_hash;
}

QString EntryInfo::path()
{
    return m_path;
}

QString EntryInfo::icon()
{
    return m_icon;
}

void EntryInfo::setValue(EntryInfo::EntryType type,const QVariant& value)
{
    switch (type){
        case EntryInfo::SizeType: 
            m_size = value.toULongLong();
            break;
        case EntryInfo::DirType:
            is_dir = value.toBool();
            break;
        case EntryInfo::HashType:
            m_hash = value.toString();
            break;
        case EntryInfo::IconType:
            m_icon = value.toString();
            break;
        case EntryInfo::PathType:
            m_path = value.toString();
            break;
        case EntryInfo::ModifiedTimeType:
            m_modifiedTime = QDateTime::fromString(value.toString());
            break;
        default:
            qDebug() << "No such type";
            return ;
    }
}
}