#include "kwallet.h"

using KWallet::Wallet;

KWalletStore::KWalletStore()
{
    m_wallet = Wallet::openWallet (Wallet::LocalWallet(), 0,
                                   Wallet::Asynchronous);
    stat = NOT_SET;
    if (m_wallet == NULL) {
        stat = NOT_AVAILABLE;
        return ;
    }
    connect (m_wallet, SIGNAL (walletOpened (bool)), SLOT (walletOpened (bool)));
}

void KWalletStore::walletOpened (bool succeeded)
{
    if (succeeded &&
            (m_wallet->hasFolder (KWallet::Wallet::FormDataFolder()) ||
             m_wallet->createFolder (KWallet::Wallet::FormDataFolder())) &&
            m_wallet->setFolder (KWallet::Wallet::FormDataFolder())
       )
        stat = SUCCEEDED;
    else
        stat = NOT_AVAILABLE;
}


bool KWalletStore::isAvailable()
{
    return stat != NOT_AVAILABLE;
}

bool KWalletStore::SetItem (const QString& key, const QString& value)
{
    if (stat == NOT_AVAILABLE)
        return false;
    if (m_wallet->hasEntry (key))
        m_wallet->removeEntry (key);
    if (m_wallet->writePassword (key, value) != 0) {
        stat = FAILED;
        return false;
    }
    stat = SUCCEEDED;
    return true;
}

bool KWalletStore::GetItem (const QString& key, QString& value)
{
    if (stat == NOT_AVAILABLE)
        return false;
    if (m_wallet->readPassword (key, value) != 0) {
        stat = FAILED;
        return false;
    }
    stat = SUCCEEDED;
    return true;
}
