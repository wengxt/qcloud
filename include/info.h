#ifndef QCLOUD_INFO_H
#define QCLOUD_INFO_H
#include <QtCore/QString>
#include <QtCore/QSettings>
#include <QtDBus/QDBusArgument>
#include <QtCloud/IPlugin>
#include <QtCloud/App>
#include "qcloud_global.h"

namespace QCloud
{
class QCLOUD_EXPORT Info
{
public:
    Info();
    Info(const App& app);
    Info(const IPlugin& app);
    const QString& name() const;
    const QString& iconName() const;
    const QString& displayName() const;
    const QString& description() const;

    void setName (const QString& name);
    void setIconName (const QString& iconName);
    void setDisplayName (const QString& displayName);
    void setDescription (const QString& description);

    static void registerMetaType();
private:
    QString m_name;
    QString m_iconName;
    QString m_displayName;
    QString m_description;
};
typedef QList<QCloud::Info> InfoList;
}

QCLOUD_EXPORT QDBusArgument& operator<<(QDBusArgument& argument, const QCloud::Info& im);
QCLOUD_EXPORT const QDBusArgument& operator>>(const QDBusArgument& argument, QCloud::Info& im);

Q_DECLARE_METATYPE(QCloud::Info)
Q_DECLARE_METATYPE(QCloud::InfoList)

#endif // QCLOUD_INFO_H
