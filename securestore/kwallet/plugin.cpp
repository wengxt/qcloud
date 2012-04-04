/* The class Definition */
#include <QObject>
#include <QtPlugin>
#include "iplugin.h"
#include "kwalletstore.h"
#include "plugin.h"

QString KWalletStorePlugin::name() const
{
    return "kwallet";
}

QString KWalletStorePlugin::displayName() const
{
    return tr ("KWallet");
}

QObject* KWalletStorePlugin::create(QObject* parent)
{
    return new KWalletStore(parent);
}

QCloud::IPlugin::PluginCategory KWalletStorePlugin::category()
{
    return QCloud::IPlugin::SecureStore;
}


Q_EXPORT_PLUGIN2 (qcloud_plainstore, KWalletStorePlugin)
