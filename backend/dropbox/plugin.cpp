/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "plugin.h"
#include "dropbox.h"

QString DropboxPlugin::name()
{
    return "dropbox";
}

QString DropboxPlugin::displayName()
{
    return tr("Dropbox");
}

QObject* DropboxPlugin::create()
{
    return new Dropbox;
}

QCloud::IPlugin::PluginCategory DropboxPlugin::category()
{
    return QCloud::IPlugin::Backend;
}

QString DropboxPlugin::iconName()
{
    return "qcloud-dropbox";
}


Q_EXPORT_PLUGIN2(qcloud_dropbox, DropboxPlugin)
