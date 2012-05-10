#include "entryinfo.h"
#include "request.h"
#include <QDebug>

namespace QCloud{
    
void EntryInfo::getInfo(const QVariantMap& value)
{
    m_size = value["bytes"].toULongLong();
    is_dir = value["is_dir"].toBool();
    m_modifiedTime = value["modified"].toString();
    m_hash = value["hash"].toString();
    m_path = value["path"].toString();
    m_icon = value["icon"].toString();
    contentsList = NULL;
}

void EntryInfo::generateContents(const QVariantMap& value)
{
    contentsList = new EntryList;
    QVariantList list = value["contents"].toList();
    Q_FOREACH (QVariant i,list){
        EntryInfo info(i.toMap());
        contentsList->append(info);
        qDebug() << info.path() << " " << info.isDir();
    }
}

EntryInfo::EntryInfo(const QString& path,IBackend* backend)
{
    QVariantMap value;
    Request* request = backend->pathInfo(path,&value);
    request->waitForFinished();
    getInfo(value);
    if (is_dir){
        generateContents(value);
    }
}

EntryInfo::EntryInfo(const QVariantMap& value)
{
    getInfo(value); 
}

EntryInfo::EntryInfo(const EntryInfo& info)
{
    if (info.contentsList!=NULL)
        contentsList = new EntryList(*info.contentsList);
    is_dir = info.is_dir;
    m_size = info.m_size;
    m_hash = info.m_hash;
    m_modifiedTime = info.m_modifiedTime;
    m_path = info.m_path;
    m_icon = info.m_icon;
    contentsList = NULL;
}

EntryInfo::~EntryInfo()
{
    delete contentsList;
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

EntryInfo::EntryError EntryInfo::getContents(EntryList& list,IBackend* backend)
{
    if (!is_dir){
        qDebug() << "Not a dir";
        return EntryInfo::NotDir;
    }
    if (contentsList!=NULL){
        list = (*contentsList);
        return EntryInfo::NoError;
    }
    if (backend==NULL){
        qDebug() << "No backend found!";
        return EntryInfo::NoBackend;
    }
    QVariantMap value;
    backend->pathInfo(m_path,&value);
    generateContents(value);
    return EntryInfo::NoError;
}
}
