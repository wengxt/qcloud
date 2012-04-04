#include <QSettings>
#include <QSet>
#include <QFileInfo>
#include <QDBusMetaType>
#include "app.h"

namespace QCloud
{

App::App (const QString& appFile)
{
    QSettings* settings = new QSettings (appFile, QSettings::IniFormat);
    m_settings.reset(settings);
    if (!settings->childGroups().contains("QCloud App"))
        return;
    QFileInfo info(appFile);
    m_name = info.fileName();
    m_displayName = settings->value ("QCloud App/Name").toString();
    m_iconName = settings->value ("QCloud App/IconName").toString();
    m_description = settings->value ("QCloud App/Description").toString();
}

bool App::isValid() const
{
    return !m_name.isEmpty();
}

QSettings* App::settings()
{
    return m_settings.data();
}

const QString& App::name() const
{
    return m_name;
}

const QString& App::iconName() const
{
    return m_iconName;
}

const QString& App::description() const
{
    return m_description;
}

const QString& App::displayName() const
{
    return m_displayName;
}

}
