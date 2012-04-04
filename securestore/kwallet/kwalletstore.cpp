#include "kwalletstore.h"
#include <QDebug>

using KWallet::Wallet;

KWalletStore::KWalletStore(QObject* parent) : ISecureStore(parent)
{
    m_wallet = Wallet::openWallet (Wallet::LocalWallet(), 0,
                                   Wallet::Synchronous);
    stat = NOT_SET;
    if ((m_wallet!=NULL) &&
            ((m_wallet->hasFolder (QCLOUD_KWALLET_FOLDER_NAME) ||
             m_wallet->createFolder (QCLOUD_KWALLET_FOLDER_NAME)) &&
              m_wallet->setFolder (QCLOUD_KWALLET_FOLDER_NAME))
       ){
        stat = SUCCEEDED;
        //qDebug() << "Currently we are in " << m_wallet->currentFolder();
        m_wallet->setFolder (QCLOUD_KWALLET_FOLDER_NAME);
    }
    else {
        qDebug() << "Could not use Wallet service, will use database to store passwords";
        stat = NOT_AVAILABLE;
    }
}

KWalletStore::~KWalletStore()
{
    delete m_wallet;
}

bool KWalletStore::isAvailable()
{
    return stat != NOT_AVAILABLE;
}

bool KWalletStore::SetItem (const QString& key, const QString& value)
{
    if (stat == NOT_AVAILABLE) {
        qDebug() << "SecureStore is not available , so cannot set";
        return false;
    }
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
    if (stat == NOT_AVAILABLE){
        qDebug() << "SecureStore is not available , so cannot get";
        return false;
    }
    stat = FAILED;
    if (m_wallet->readPassword (key, value) != 0) {
        return false;
    }
    if (value=="")
        return false;
    stat = SUCCEEDED;
    return true;
}
