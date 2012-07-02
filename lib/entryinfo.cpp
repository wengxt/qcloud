#include <QDebug>
#include <QDBusMetaType>
#include "entryinfo.h"
#include "request.h"

namespace QCloud{

void EntryInfo::registerMetaType()
{
    qRegisterMetaType<EntryInfo>("QCloud::EntryInfo");
    qDBusRegisterMetaType<EntryInfo>();
    qRegisterMetaType<EntryInfoList>("QCloud::EntryInfoList");
    qDBusRegisterMetaType<EntryInfoList>();
}

EntryInfo::~EntryInfo()
{

}

EntryInfo::EntryInfo()
{

}

qulonglong EntryInfo::size() const
{
    return m_size;
}

bool EntryInfo::isDir() const
{
    return is_dir;
}

const QDateTime& EntryInfo::modifiedTime() const
{
    return m_modifiedTime;
}

const QString& EntryInfo::hash() const
{
    return m_hash;
}

const QString& EntryInfo::path() const
{
    return m_path;
}

const QString& EntryInfo::icon() const
{
    return m_icon;
}

const QString& EntryInfo::mime() const
{
    return m_mime;
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
            m_modifiedTime = value.toDateTime();
            break;
        case EntryInfo::MimeType:
            m_mime = value.toString();
            break;
        default:
            qDebug() << "No such type";
            return ;
    }
}
}

QDBusArgument& operator<<(QDBusArgument& argument, const QCloud::EntryInfo& info)
{
    argument.beginStructure();
    argument << info.path();
    argument << (quint64) info.modifiedTime().toTime_t();
    argument << info.size();
    argument << info.hash();
    argument << info.isDir();
    argument << info.icon();
    argument << info.mime();
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, QCloud::EntryInfo& info)
{
    QString path;
    quint64 modifiedTime;
    quint64 size;
    QString hash;
    bool isDir;
    QString icon;
    QString mime;
    argument.beginStructure();
    argument >> path >> modifiedTime >> size >> hash >> isDir >> icon >> mime;
    argument.endStructure();
    info.setValue(QCloud::EntryInfo::PathType, path);
    info.setValue(QCloud::EntryInfo::ModifiedTimeType, QDateTime::fromTime_t(modifiedTime));
    info.setValue(QCloud::EntryInfo::SizeType, size);
    info.setValue(QCloud::EntryInfo::HashType, hash);
    info.setValue(QCloud::EntryInfo::DirType, isDir);
    info.setValue(QCloud::EntryInfo::IconType, icon);
    info.setValue(QCloud::EntryInfo::MimeType, mime);
    return argument;
}