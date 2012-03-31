/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "kdenetworkaccessmanager.h"
#include "plugin.h"

QString KDENetworkPlugin::name()
{
    return "kde";
}

QString KDENetworkPlugin::displayName()
{
    return tr ("KDE");
}

QObject* KDENetworkPlugin::create()
{
    return new KDENetworkAccessManager;
}

QCloud::IPlugin::PluginCategory KDENetworkPlugin::category()
{
    return QCloud::IPlugin::Network;
}


Q_EXPORT_PLUGIN2 (qcloud_kdenetwork, KDENetworkPlugin)
