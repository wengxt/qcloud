/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "plainstore.h"
#include "plugin.h"

QString PlainStorePlugin::name() const
{
    return "plain";
}

QString PlainStorePlugin::displayName() const
{
    return tr ("Plain");
}

QObject* PlainStorePlugin::create(QObject* parent)
{
    return new PlainStore(parent);
}

QCloud::IPlugin::PluginCategory PlainStorePlugin::category()
{
    return QCloud::IPlugin::SecureStore;
}


Q_EXPORT_PLUGIN2 (qcloud_plainstore, PlainStorePlugin)
