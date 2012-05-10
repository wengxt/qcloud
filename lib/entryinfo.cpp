#include "entryinfo.h"
#include "request.h"
#include <QDebug>

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

QString EntryInfo::modifiedTime()
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

bool EntryInfo::getContents(EntryList& list)
{
    //qDebug() << m_hash << " " << m_icon << " "<< m_modifiedTime << " " << m_path << " " << m_size;
    if (!is_dir){
        qDebug() << "Not a dir";
        return false;
    }
    list = contentsList;
    return true;
}

void EntryInfo::setContents(const EntryList& list)
{
    contentsList = list;
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
            m_modifiedTime = value.toString();
            break;
        default:
            qDebug() << "No such type";
            return ;
    }
}
}