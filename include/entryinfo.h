#ifndef QCLOUD_ENTRYINFO_H
#define QCLOUD_ENTRYINFO_H
#include <QObject>
#include <QMap>
#include <QVariant>
#include <QDateTime>
#include "qcloud_global.h"
#include "ibackend.h"


namespace QCloud{
class EntryInfo;
typedef QList<EntryInfo> EntryList;

class QCLOUD_EXPORT EntryInfo
{
public:
    enum EntryType{
        SizeType,
        DirType,
        HashType,
        IconType,
        PathType,
        ModifiedTimeType
    };
    EntryInfo();
    virtual ~EntryInfo();
    qulonglong size();
    bool isDir();
    QDateTime modifiedTime();
    QString hash();
    QString path();
    QString icon();
    void setValue(EntryType type,const QVariant& value);
    void setContents(const EntryList& list);
    bool getContents(EntryList& list);
protected:
    qulonglong m_size;
    bool is_dir;
    QString m_hash,m_path,m_icon;
    QDateTime m_modifiedTime;
    EntryList contentsList;
};
}
#endif