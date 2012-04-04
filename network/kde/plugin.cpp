/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "kdenetworkaccessmanager.h"
#include "plugin.h"

QString KDENetworkPlugin::name() const
{
    return "kde";
}

QString KDENetworkPlugin::displayName() const
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
