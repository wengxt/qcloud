#include "app.h"
#include <QSettings>
#include <qset.h>

namespace QCloud {
App::App(const QSettings& settings)
{
    m_name = settings.value("Name").toString();
    m_iconName = settings.value("IconName").toString();
    m_description = settings.value("Description").toString();
}

const QString& App::name()
{
    return m_name;
}

const QString& App::iconName()
{
    return m_iconName;
}

const QString& App::description()
{
    return m_description;
}

App* App::parseAppFile(const QString& appFile)
{
    QSettings settings(appFile, QSettings::IniFormat);
    settings.beginGroup("QCloud App");
    if (settings.contains("Name")
     && settings.contains("IconName")
     && settings.contains("Description"))
    {
        App* app = new App(settings);
        return app;
    }
    return NULL;
}

}