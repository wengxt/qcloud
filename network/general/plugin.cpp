/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "networkaccessmanager.h"
#include "plugin.h"

QString GeneralNetworkPlugin::name() const
{
    return "general";
}

QString GeneralNetworkPlugin::displayName() const
{
    return tr ("General");
}

QObject* GeneralNetworkPlugin::create(QObject* parent)
{
    return new NetworkAccessManager(parent);
}

QCloud::IPlugin::PluginCategory GeneralNetworkPlugin::category()
{
    return QCloud::IPlugin::Network;
}


Q_EXPORT_PLUGIN2 (qcloud_generalnetwork, GeneralNetworkPlugin)
