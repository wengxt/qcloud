#include "encrypter.h"
#include "isecurestore.h"
#include <QtCrypto/QtCrypto>
#include <QFile>
#include <QDebug>
#define KEY_NAME "QCloud_Encrypter_Key"
#define IV_NAME "Qcloud_Encrypter_Initiazation_Vector"
#define KEY_LEN 16
#define IV_LEN 16
#define BUF_SIZE 16
#define PADDING NoPadding
#define ENC_TYPE CFB

namespace QCloud{
    
inline bool Encrypter::init()
{
   if (!hasKey){
       qDebug() << "Key not found , assume it is the first time to run encrypt/decrypt";
        QString key_value,iv_value;
        if ((!m_storage->GetItem(KEY_NAME,key_value)) || (!m_storage->GetItem(IV_NAME,iv_value))){
            qDebug() << "Failed getting key or iv , generating new value and new iv";
            key = QCA::SymmetricKey(KEY_LEN);
            iv = QCA::InitializationVector(IV_LEN);
            qDebug() << "Finished generating. key : " << key.toByteArray() << "iv : " << iv.toByteArray() << ". Starting writing value to ISecureStore";
            bool flag = m_storage->SetItem(KEY_NAME,key.data());
            flag &= m_storage->SetItem(IV_NAME,iv.data());
            qDebug() << "Finished Writing key and iv";
            if (!flag){
                qDebug() << "Failed setting one of the items";
                return false;
            }
        }
        else{
            key = QCA::SymmetricKey(key_value.toLocal8Bit());
            iv = QCA::InitializationVector(iv_value.toLocal8Bit());
        }
        hasKey = true;
        qDebug() << "Set hasKey to True";
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
    qDebug() << "Encrypting " << fileName << " to " << outputFile;
    if (!init())
        return false;
    QFile readFile(fileName);
    if (!readFile.exists()){
        qDebug() << "Input file \'" << fileName << "\' Not found";
        readFile.close();
        return false;
    }
    if (!readFile.open(QIODevice::ReadOnly)){
        qDebug() << "Open input \'" << fileName << "\' failed";
        readFile.close();
        return false;
    }
    QFile writeFile(outputFile);
    if (!writeFile.open(QIODevice::WriteOnly)){
        qDebug() << "Open output \'" << outputFile << "\' failed";
        writeFile.close();
        return false;
    }
    
    QCA::Cipher cipher("aes128",QCA::Cipher::ENC_TYPE,
        //NoPadding with CFB or DefaultPadding with CBC
        QCA::Cipher::PADDING,
        QCA::Encode,
        key,iv);
    
    QByteArray buf;
    buf = readFile.read(BUF_SIZE);
    //qDebug() << "Reading First buffer : " << buf;
    while (buf.size()>0){
        QCA::SecureArray bufRegion;
        QCA::SecureArray bufWrite;
        bufRegion = QCA::SecureArray(buf);
        //qDebug() << "Changing buffer : " << bufRegion.toByteArray();
        cipher.setup(QCA::Encode,key,iv);
        bufWrite = cipher.process(bufRegion);
        if (!cipher.ok()){
            qDebug() << "Error with cipher.process while encrypting";
            return false;
        }
        //qDebug() << "Writing data : " << bufWrite.toByteArray();
        if (writeFile.write(bufWrite.toByteArray())==-1){
            qDebug() << "Error while writing encrypted data";
            return false;
        }
        buf = readFile.read(BUF_SIZE);
        //qDebug() << "Read buffer : " << buf;
    }
    
    readFile.close();
    writeFile.close();
    qDebug() << "Finished encrypting";
    return true;
}

bool Encrypter::decrypt(const QString& fileName,const QString& outputFile)
{
    qDebug() << "Decrypting " << fileName << " to " << outputFile;
    if (!init())
        return false;
    QFile readFile(fileName);
    if (!readFile.exists()){
        qDebug() << "Input file \'" << fileName << "\' Not found";
        readFile.close();
        return false;
    }
    if (!readFile.open(QIODevice::ReadOnly)){
        qDebug() << "Open input \'" << fileName << "\' failed";
        readFile.close();
        return false;
    }
    QFile writeFile(outputFile);
    if (!writeFile.open(QIODevice::WriteOnly)){
        qDebug() << "Open output \'" << outputFile << "\' failed";
        writeFile.close();
        return false;
    }
    
    QCA::Cipher cipher("aes128",QCA::Cipher::ENC_TYPE,
        //NoPadding with CFB or DefaultPadding with CBC
        QCA::Cipher::PADDING,
        QCA::Decode,
        key,iv);
    
    QByteArray buf;
    buf = readFile.read(BUF_SIZE);
    //qDebug() << "Reading First buffer : " << buf;
    while (buf.size()>0){
        QCA::SecureArray bufRegion;
        QCA::SecureArray bufWrite;
        cipher.setup(QCA::Decode,key,iv);
        bufRegion = QCA::SecureArray(buf);
        bufWrite = cipher.process(bufRegion);
        if (!cipher.ok()){
            qDebug() << "Error with cipher.process while decrypting";
            return false;
        }
        if (writeFile.write(bufWrite.toByteArray())==-1){
            qDebug() << "Error while writing decrypted data";
            return false;
        }
        buf = readFile.read(BUF_SIZE);
        //qDebug() << "Reading buffer : " << buf;
    }
    
    readFile.close();
    writeFile.close();
    qDebug() << "Finished decrypting";
    return true;
}

}