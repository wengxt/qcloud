/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "plugin.h"
#include "dropbox.h"

QString DropboxPlugin::name() const
{
    return "dropbox";
}

QString DropboxPlugin::displayName() const
{
    return tr ("Dropbox");
}

QObject* DropboxPlugin::create(QObject* parent)
{
    return new Dropbox(parent);
}

QCloud::IPlugin::PluginCategory DropboxPlugin::category()
{
    return QCloud::IPlugin::Backend;
}

QString DropboxPlugin::iconName() const
{
    return "qcloud-dropbox";
}


Q_EXPORT_PLUGIN2 (qcloud_dropbox, DropboxPlugin)
