#ifndef KWALLET_H
#define KWALLET_H

#include "isecurestore.h"
#define QCLOUD_KWALLET_FOLDER_NAME "QCloud"

namespace KWallet {
class Wallet;
}

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

    virtual bool isAvailable();

    virtual bool readItem (const QString& group, const QString& key, QByteArray& value);
    virtual bool writeItem (const QString& group, const QString& key, const QByteArray& value);
    virtual bool deleteItem (const QString& group, const QString& key);

private:
    KWallet::Wallet* m_wallet;
    KWALLET_STAT stat;
};

#endif
