#include "kwallet.h"
#include "isecurestorewidget.h"
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QString>
#include <QApplication>
#include <iostream>

using namespace QCloud;

KWalletStore storage;

int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    KWalletStore wallet;
    ISecureStoreWidget test_widget(&wallet);
    app.exec();
    return 0;
}