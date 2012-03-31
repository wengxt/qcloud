#ifndef QCLOUD_APP_H
#define QCLOUD_APP_H
#include <QtCore/QString>
#include <QtCore/QSettings>
#include "qcloud_global.h"

namespace QCloud
{
class QCLOUD_EXPORT App
{
public:
    App (const QSettings& settings);
    const QString& name();
    const QString& iconName();
    const QString& description();

    static App* parseAppFile (const QString& parseAppFile);

private:
    QString m_name;
    QString m_iconName;
    QString m_description;
};
}

#endif
