#include <QSettings>
#include <QSet>
#include <QDBusMetaType>
#include "info.h"

namespace QCloud
{
void Info::registerMetaType()
{
    qRegisterMetaType<Info>("QCloud::Info");
    qDBusRegisterMetaType<Info>();
    qRegisterMetaType<InfoList>("QCloud::InfoList");
    qDBusRegisterMetaType<InfoList>();
}

Info::Info()
{
}

Info::Info (const App& app) {
    m_name = app.name();
    m_displayName = app.displayName();
    m_iconName = app.iconName();
    m_description = app.description();
}

Info::Info (const IPlugin& iplugin) {
    m_name = iplugin.name();
    m_displayName = iplugin.displayName();
    m_iconName = iplugin.iconName();
    m_description = iplugin.description();
}

const QString& Info::name() const
{
    return m_name;
}

const QString& Info::iconName() const
{
    return m_iconName;
}

const QString& Info::displayName() const
{
    return m_displayName;
}

const QString& Info::description() const
{
    return m_description;
}

void Info::setName (const QString& name)
{
    m_name = name;
}

void Info::setIconName (const QString& iconName)
{
    m_iconName = iconName;
}

void Info::setDescription (const QString& description)
{
    m_description = description;
}

void Info::setDisplayName (const QString& displayName)
{
    m_displayName = displayName;
}

}



QDBusArgument& operator<<(QDBusArgument& argument, const QCloud::Info& info)
{
    argument.beginStructure();
    argument << info.name();
    argument << info.iconName();
    argument << info.displayName();
    argument << info.description();
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, QCloud::Info& info)
{
    QString name, iconName, description, displayName;
    argument.beginStructure();
    argument >> name >> iconName >> displayName >> description;
    argument.endStructure();
    info.setName(name);
    info.setIconName(iconName);
    info.setDisplayName(displayName);
    info.setDescription(description);
    return argument;
}
