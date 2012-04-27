#include "encrypter.h"
#include "isecurestore.h"
#include <QtCrypto/QtCrypto>
#include <QFile>
#include <QDebug>
#include <QInputDialog>
#define KEY_LEN 16
#define IV_LEN 16
#define BUF_SIZE 8192
#define PADDING NoPadding
#define ENC_TYPE CFB
#define ENCRYPTER_TEST_CONTENT "abcdefghijklmnopqrstuvwxyz0123456789"

namespace QCloud {

inline static void clearString(QString& st)
{
    for (QString::Iterator it=st.begin(); it!=st.end(); it++)
        (*it) = '\0';
    st = "";
}

void Encrypter::generateKey(QCA::SymmetricKey& key)
{
    //key = QCA::SymmetricKey(KEY_LEN);
    qDebug() << "Setting generator";
    QCA::PBKDF2 generator("sha1");
    qDebug() << "Set generator";
    key = generator.makeKey(
              QCA::SecureArray(QInputDialog::getText(NULL,tr("Input Password"),
                               tr("Password"),
                               QLineEdit::PasswordEchoOnEdit).toAscii()),
              QCA::InitializationVector(),KEY_LEN,10000);
}

inline bool Encrypter::init()
{
    if (m_storage==NULL || (!m_storage->isAvailable())) {
        qDebug() << "SecureStore is not available";
        return false;
    }
    if (!hasKey) {
        qDebug() << "Key not found , assume it is the first time to run encrypt/decrypt";
        QString key_value;
        if (!m_storage->GetItem(KEY_NAME,key_value)) {
            qDebug() << "Failed getting key , generating new value from user input";
            generateKey(key);
            qDebug() << "Finished generating.";
            bool flag = m_storage->SetItem(KEY_NAME,key.toByteArray());
            qDebug() << "Finished Writing key";
            if (!flag) {
                qDebug() << "Failed setting one of the items";
                return false;
            }
        }
        else {
            qDebug() << "Successfully got key from SecureStore";
            key = QCA::SymmetricKey(key_value.toAscii());
        }
        /*Clear the QString values to prevent them from being stolen by other program ,
         *    even if it might not help at all.*/
        clearString(key_value);
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
    QFile readFile(fileName);
    if (!readFile.exists()) {
        qDebug() << "Input file \'" << fileName << "\' Not found";
        readFile.close();
        return false;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Open input \'" << fileName << "\' failed";
        readFile.close();
        return false;
    }
    QFile writeFile(outputFile);
    if (!writeFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Open output \'" << outputFile << "\' failed";
        writeFile.close();
        return false;
    }
    QCA::InitializationVector iv = QCA::InitializationVector(IV_LEN);
    writeFile.write(iv.toByteArray());
    if (!init())
        return false;

    QCA::Cipher cipher("aes128",QCA::Cipher::ENC_TYPE,
                       //NoPadding with CFB or DefaultPadding with CBC
                       QCA::Cipher::PADDING,
                       QCA::Encode,
                       key,iv);

    QByteArray buf;
    writeFile.write(cipher.process(QCA::SecureArray(ENCRYPTER_TEST_CONTENT)).toByteArray());
    buf = readFile.read(BUF_SIZE);
    //qDebug() << "Reading First buffer : " << buf;
    while (buf.size()>0) {
        QCA::SecureArray bufRegion;
        QCA::SecureArray bufWrite;
        bufRegion = QCA::SecureArray(buf);
        //qDebug() << "Changing buffer : " << bufRegion.toByteArray();
        bufWrite = cipher.process(bufRegion);
        if (!cipher.ok()) {
            qDebug() << "Error with cipher.process while encrypting";
            return false;
        }
        //qDebug() << "Writing data : " << bufWrite.toByteArray();
        if (writeFile.write(bufWrite.toByteArray())==-1) {
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
    QFile readFile(fileName);
    if (!readFile.exists()) {
        qDebug() << "Input file \'" << fileName << "\' Not found";
        readFile.close();
        return false;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Open input \'" << fileName << "\' failed";
        readFile.close();
        return false;
    }
    QFile writeFile(outputFile);
    if (!writeFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Open output \'" << outputFile << "\' failed";
        writeFile.close();
        return false;
    }

    QCA::InitializationVector iv = QCA::InitializationVector(readFile.read(IV_LEN));
    qDebug() << "Read IV " << QCA::arrayToHex(iv.toByteArray());
    if (iv.size()!=IV_LEN) {
        qDebug() << "Error while reading IV from file!";
        return false;
    }

    QCA::Cipher cipher("aes128",QCA::Cipher::ENC_TYPE,
                       //NoPadding with CFB or DefaultPadding with CBC
                       QCA::Cipher::PADDING,
                       QCA::Decode,
                       key,iv);

    QByteArray buf;
    QCA::SecureArray bufRegion;
    buf = readFile.read(sizeof(ENCRYPTER_TEST_CONTENT) - 1);
    int cnt = 0;
    qDebug() << "Checking password , original content is " << QCA::arrayToHex(ENCRYPTER_TEST_CONTENT);
    while (cnt<3) {
        if (!init()) {
            readFile.close();
            writeFile.close();
            return false;
        }
        cipher.setup(QCA::Decode,key,iv);
        bufRegion = cipher.process(QCA::SecureArray(buf));
        if ((bufRegion.toByteArray())==ENCRYPTER_TEST_CONTENT) {
            qDebug() << "Password checked ok!";
            break;
        }
        hasKey = false;
        m_storage->SetItem(KEY_NAME,"");
        qDebug() << "Password incorrect! Decrypted content is " << QCA::arrayToHex(bufRegion.toByteArray());
        cnt ++;
    }
    if (cnt>=3) {
        qDebug() << "Got wrong key for 3 times , return";
        readFile.close();
        writeFile.close();
        return false;
    }

    buf = readFile.read(BUF_SIZE);
    //qDebug() << "Reading First buffer : " << buf;
    while (buf.size()>0) {
        QCA::SecureArray bufWrite;
        bufRegion = QCA::SecureArray(buf);
        bufWrite = cipher.process(bufRegion);
        if (!cipher.ok()) {
            qDebug() << "Error with cipher.process while decrypting";
            return false;
        }
        if (writeFile.write(bufWrite.toByteArray())==-1) {
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

