#ifndef QCLOUD_ENCRYPTER_H
#define QCLOUD_ENCRYPTER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include "isecurestore.h"
#include <QtCrypto/QtCrypto>
#include "qcloud_global.h"

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
    
    bool hasKey;
    ISecureStore *m_storage;
    QCA::SymmetricKey key;
    QCA::InitializationVector iv;
};

}

#endif