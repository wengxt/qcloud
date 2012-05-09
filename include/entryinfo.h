#ifndef QCLOUD_ENTRYINFO_H
#define QCLOUD_ENTRYINFO_H
#include "qcloud_global.h"
#include "ibackend.h"
#include <QObject>
#include <QMap>
#include <QVariant>


namespace QCloud{
class EntryInfo;
typedef QList<EntryInfo> EntryList;

class QCLOUD_EXPORT EntryInfo
{
public:
    enum EntryError{
        NoError,
        NotDir,
        NoBackend
    };
    EntryInfo(const QString& path,IBackend* backend);
    EntryInfo(const QVariantMap& value);
    EntryInfo(const EntryInfo& info);
    virtual ~EntryInfo();
    qulonglong size();
    bool isDir();
    QString modifiedTime();
    QString hash();
    QString path();
    QString icon();
    EntryError getContents(EntryList& list,IBackend* backend = NULL);
protected:
    qulonglong m_size;
    bool is_dir;
    QString m_modifiedTime,m_hash,m_path,m_icon;
    EntryList* contentsList;
private:
    void getInfo(const QVariantMap& value);
    void generateContents(const QVariantMap& value);
};
}
#endif