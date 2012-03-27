/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "networkaccessmanager.h"
#include "plugin.h"

QString GeneralNetworkPlugin::name()
{
    return "general";
}

QString GeneralNetworkPlugin::displayName()
{
    return tr("General");
}

QObject* GeneralNetworkPlugin::create()
{
    return new NetworkAccessManager;
}

QCloud::IPlugin::PluginCategory GeneralNetworkPlugin::category()
{
    return QCloud::IPlugin::Network;
}


Q_EXPORT_PLUGIN2(qcloud_generalnetwork, GeneralNetworkPlugin)
