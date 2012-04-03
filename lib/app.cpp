#include "app.h"
#include <QSettings>
#include <qset.h>

namespace QCloud
{
App::App (const QString& appFile)
{
    QSettings* settings = new QSettings (appFile, QSettings::IniFormat);
    m_settings.reset (settings);
    m_name = m_settings->value ("QCloud App/Name").toString();
    m_iconName = m_settings->value ("QCloud App/IconName").toString();
    m_description = m_settings->value ("QCloud App/Description").toString();
}

bool App::isValid() const
{
    return !m_name.isEmpty();
}

const QSettings& App::settings() const
{
    return *m_settings;
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

}
