#include <QDebug>
#include <QCoreApplication>

#include "encrypter.h"
#include "kwalletstore.h"

using namespace QCloud;

int main(int argc,char **argv)
{
    if (argc<4){
        qDebug() << "Usage : encrypt/decrypt inputFile outputFile";
        return 1;
    }
    QCoreApplication app(argc,argv);
    //QCA::Initializer init;
    KWalletStore *storage = new KWalletStore;
    Encrypter *encrypter = new Encrypter(storage);
    if (argv[1]==QString("encrypt")){
        qDebug() << "Starting to encryt" << argv[2] << "to " << argv[3];
        encrypter->encrypt(argv[2],argv[3]);
    }
    else{
        qDebug() << "Starting to decrypt" << argv[2] << "to " << argv[3];
        encrypter->decrypt(argv[2],argv[3]);
    }
    return 0;
}