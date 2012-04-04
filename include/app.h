#ifndef QCLOUD_APP_H
#define QCLOUD_APP_H
#include <QtCore/QString>
#include <QtCore/QSettings>
#include <QtDBus/QDBusArgument>
#include "qcloud_global.h"

namespace QCloud
{
class QCLOUD_EXPORT App
{
public:
    App (const QString& appFile);
    bool isValid() const;
    const QString& name() const;
    const QString& iconName() const;
    const QString& description() const;
    const QString& displayName() const;
    QSettings* settings();

private:
    QString m_name;
    QString m_iconName;
    QString m_description;
    QString m_displayName;
    QScopedPointer<QSettings> m_settings;
};
}

#endif
