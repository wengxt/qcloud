/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "plainstore.h"
#include "plugin.h"

QString PlainStorePlugin::name()
{
    return "plain";
}

QString PlainStorePlugin::displayName()
{
    return tr ("Plain");
}

QObject* PlainStorePlugin::create()
{
    return new PlainStore;
}

QCloud::IPlugin::PluginCategory PlainStorePlugin::category()
{
    return QCloud::IPlugin::SecureStore;
}


Q_EXPORT_PLUGIN2 (qcloud_plainstore, PlainStorePlugin)
