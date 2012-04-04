#ifndef KWALLET_H
#define KWALLET_H

#include "isecurestore.h"
#include <KWallet/Wallet>
#define QCLOUD_KWALLET_FOLDER_NAME "QCloud"

enum KWALLET_STAT {
    NOT_SET,
    SUCCEEDED,
    FAILED,
    NOT_AVAILABLE
};

class KWalletStore :  public QCloud::ISecureStore
{
    Q_OBJECT;
public:

    explicit KWalletStore (QObject* parent = 0);
    virtual ~KWalletStore();

    bool isAvailable();

    bool GetItem (const QString& key, QString& value);
    bool SetItem (const QString& key, const QString& value);

private:
    KWallet::Wallet* m_wallet;
    KWALLET_STAT stat;
};

#endif
