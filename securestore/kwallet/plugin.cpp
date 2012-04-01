/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "kwalletstore.h"
#include "plugin.h"

QString KWalletStorePlugin::name()
{
    return "kwallet";
}

QString KWalletStorePlugin::displayName()
{
    return tr ("KWallet");
}

QObject* KWalletStorePlugin::create()
{
    return new KWalletStore;
}

QCloud::IPlugin::PluginCategory KWalletStorePlugin::category()
{
    return QCloud::IPlugin::SecureStore;
}


Q_EXPORT_PLUGIN2 (qcloud_plainstore, KWalletStorePlugin)
