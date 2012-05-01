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

bool KWalletStore::writeItem (const QString& group, const QString& key, const QByteArray& value)
{
    if (stat == NOT_AVAILABLE) {
        qDebug() << "SecureStore is not available , so cannot set";
        return false;
    }

    do {
        QString kwalletKey = QString("%1_%2").arg(group).arg(key);
        if (m_wallet->hasEntry (kwalletKey))
            m_wallet->removeEntry (kwalletKey);
        if (m_wallet->writeEntry (kwalletKey, value) != 0)
            break;
        stat = SUCCEEDED;
        return true;
    } while(0);
    stat = FAILED;
    return false;
}

bool KWalletStore::readItem (const QString& group, const QString& key, QByteArray& value)
{
    if (stat == NOT_AVAILABLE){
        qDebug() << "SecureStore is not available , so cannot get";
        return false;
    }
    stat = FAILED;
    QString kwalletKey = QString("%1_%2").arg(group).arg(key);
    if (m_wallet->readEntry (kwalletKey, value) != 0) {
        return false;
    }
    stat = SUCCEEDED;
    return true;
}

bool KWalletStore::deleteItem (const QString& group, const QString& key)
{
    if (stat == NOT_AVAILABLE){
        qDebug() << "SecureStore is not available , so cannot get";
        return false;
    }
    stat = FAILED;
    QString kwalletKey = QString("%1_%2").arg(group).arg(key);
    if (m_wallet->removeEntry (kwalletKey) != 0) {
        return false;
    }
    stat = SUCCEEDED;
    return true;
}
