#ifndef QCLOUD_ENCRYPTER_P_H
#define QCLOUD_ENCRYPTER_P_H

#include "encrypter.h"
#include <QtCrypto>

namespace QCloud
{
class EncrypterPrivate {
public:
    explicit EncrypterPrivate(Encrypter* encrypter);
    virtual ~EncrypterPrivate();

    bool init();
    void generateKey(QCA::SymmetricKey& key);
    bool hasKey;
    ISecureStore *storage;
    QCA::SymmetricKey key;
    QCA::Initializer initQCA;
    Encrypter* p;

};

}

#endif // QCLOUD_IBACKEND_P_H