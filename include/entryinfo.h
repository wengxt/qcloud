#ifndef QCLOUD_ENTRYINFO_H
#define QCLOUD_ENTRYINFO_H
#include <QObject>
#include <QMap>
#include <QVariant>
#include <QDateTime>
#include <QDBusArgument>
#include "qcloud_global.h"


namespace QCloud{

class QCLOUD_EXPORT EntryInfo
{
public:
    enum EntryType{
        SizeType,
        DirType,
        HashType,
        IconType,
        PathType,
        ModifiedTimeType,
        MimeType
    };
    EntryInfo();
    virtual ~EntryInfo();
    static void registerMetaType();

    qulonglong size() const;
    bool isDir() const;
    const QDateTime& modifiedTime() const;
    const QString& hash() const;
    const QString& path() const;
    const QString& icon() const;
    const QString& mime() const;
    void setValue(EntryType type,const QVariant& value);
protected:
    qulonglong m_size;
    bool is_dir;
    QString m_hash,m_path,m_icon,m_mime;
    QDateTime m_modifiedTime;
};
typedef QList<EntryInfo> EntryInfoList;
}

QCLOUD_EXPORT QDBusArgument& operator<<(QDBusArgument& argument, const QCloud::EntryInfo& im);
QCLOUD_EXPORT const QDBusArgument& operator>>(const QDBusArgument& argument, QCloud::EntryInfo& im);

Q_DECLARE_METATYPE(QCloud::EntryInfo)
Q_DECLARE_METATYPE(QCloud::EntryInfoList)
#endif