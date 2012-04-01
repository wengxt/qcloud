#include "encrypter.h"
#include "isecurestore.h"
#include <QtCrypto/QtCrypto>
#include <QFile>
#include <QDebug>
#define KEY_NAME "QCloud_Encrypter_Key"
#define IV_NAME "Qcloud_Encrypter_Initiazation_Vector"
#define KEY_LEN 16
#define IV_LEN 16
#define BUF_SIZE 8192

namespace QCloud{
    
inline bool Encrypter::init()
{
    if (!hasKey){
        QString key_value,iv_value;
        if ((!m_storage->GetItem(KEY_NAME,key_value)) || (!m_storage->GetItem(IV_NAME,iv_value))){
            qDebug() << "Failed getting key or iv , generating new value and new iv";
            //key = QCA::SymmetricKey(KEY_LEN);
            //iv = QCA::InitializationVector(IV_LEN);
            QCA::SymmetricKey key1(KEY_LEN);
            qDebug() << "Finished generating. key : " << key.constData() << "iv : " << iv.constData() << ". Starting writing value to ISecureStore";
            return false;
            bool flag = m_storage->SetItem(KEY_NAME,key.data());
            flag &= m_storage->SetItem(IV_NAME,iv.data());
            qDebug() << "Finished Writing";
            if (!flag){
                qDebug() << "Failed Setting Item";
                return false;
            }
            key_value = "";
            iv_value = "";
            hasKey = true;
        }
        else{
            key = QCA::SymmetricKey(key_value.toAscii());
            iv = QCA::InitializationVector(iv_value.toAscii());
        }
    }
    return true;
}
    
Encrypter::Encrypter(ISecureStore *storage)
{
    m_storage = storage;
    hasKey = false;
}

Encrypter::~Encrypter()
{
    
}

bool Encrypter::encrypt(const QString& fileName,const QString& outputFile)
{
    if (!init())
        return false;
    QFile readFile(fileName);
    if (!readFile.exists()){
        qDebug() << "File \'" << fileName << "\' Not found";
        return false;
    }
    if (!readFile.open(QIODevice::ReadOnly)){
        qDebug() << "Open \'" << fileName << "\' failed";
        return false;
    }
    QFile writeFile(fileName);
    if (!writeFile.exists()){
        qDebug() << "File \'" << fileName << "\' Not found";
        return false;
    }
    if (!writeFile.open(QIODevice::WriteOnly)){
        qDebug() << "Open \'" << fileName << "\' failed";
        return false;
    }
    
    QCA::Cipher cipher("aes128",QCA::Cipher::CFB,
        //NoPadding with CFB or DefaultPadding with CBC
        QCA::Cipher::DefaultPadding,
        QCA::Encode,
        key,iv);
    
    QByteArray buf;
    buf = readFile.read(BUF_SIZE);
    while (buf.size()>0){
        QCA::SecureArray bufRegion(buf);
        QCA::SecureArray bufWrite = cipher.update(bufRegion);
        writeFile.write(bufWrite.append(cipher.final()).constData());
        buf = readFile.read(BUF_SIZE);
    }
    
    readFile.close();
    writeFile.close();
    return true;
}

bool Encrypter::decrypt(const QString& fileName,const QString& outputFile)
{
    if (!init())
        return false;
     QFile readFile(fileName);
    if (!readFile.exists()){
        qDebug() << "File \'" << fileName << "\' Not found";
        return false;
    }
    if (!readFile.open(QIODevice::ReadOnly)){
        qDebug() << "Open \'" << fileName << "\' failed";
        return false;
    }
    QFile writeFile(fileName);
    if (!writeFile.exists()){
        qDebug() << "File \'" << fileName << "\' Not found";
        return false;
    }
    if (!writeFile.open(QIODevice::WriteOnly)){
        qDebug() << "Open \'" << fileName << "\' failed";
        return false;
    }
    
    QCA::Cipher cipher("aes128",QCA::Cipher::CBC,
        //NoPadding with CFB or DefaultPadding with CBC
        QCA::Cipher::DefaultPadding,
        QCA::Decode,
        key,iv
    );
    
    QByteArray buf;
    buf = readFile.read(BUF_SIZE);
    while (buf.size()>0){
        QCA::SecureArray bufRegion(buf);
        QCA::SecureArray bufWrite = cipher.update(buf);
        writeFile.write(bufWrite.append(cipher.final()).constData());
        buf = readFile.read(BUF_SIZE);
    }
    
    readFile.close();
    writeFile.close();
    return true;
}

}