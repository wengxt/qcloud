#ifndef QCLOUD_ENCRYPTER_H
#define QCLOUD_ENCRYPTER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCrypto/QtCrypto>
#include "isecurestore.h"
#include "qcloud_global.h"
#define GROUP_NAME "QCloud_Encrypter"
#define KEY_NAME "Key"

namespace QCloud{

class QCLOUD_EXPORT Encrypter : public QObject
{
    Q_OBJECT;
public:
    Encrypter(ISecureStore *storage);
    virtual ~Encrypter();
    bool encrypt(const QString& fileName,const QString& outputFile);
    bool decrypt(const QString& fileName,const QString& outputFile);
private:
    bool init();
    void generateKey(QCA::SymmetricKey& key);

    bool hasKey;
    ISecureStore *m_storage;
    QCA::SymmetricKey key;
    QCA::Initializer initQCA;
};

}

#endif