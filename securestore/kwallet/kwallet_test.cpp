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
    KWalletStore *wallet = new KWalletStore;
    ISecureStoreWidget test_widget(wallet);
    test_widget.show();
    app.exec();
    return 0;
}