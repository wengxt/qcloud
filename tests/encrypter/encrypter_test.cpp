#include <QDebug>

#include "encrypter.h"
#include "kwalletstore.h"

using namespace QCloud;

int main(int argc,char **argv)
{
    if (argc<4)
        return 1;
    qDebug() << "File to encrypt : " << argv[1];
    qDebug() << "Encrypt output : " << argv[2];
    qDebug() << "Decrypt output : " << argv[3];
    KWalletStore *storage = new KWalletStore;
    Encrypter *encrypter = new Encrypter(storage);
    qDebug() << "Starting to encryt" << argv[1];
    encrypter->encrypt(argv[1],argv[2]);
    qDebug() << "Starting to decrypt" << argv[2];
    encrypter->decrypt(argv[2],argv[3]);
}