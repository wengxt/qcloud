#include "kwallet.h"
#include "securestorewidget.h"
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QString>
#include <QApplication>
#include <iostream>

using namespace QCloud;

class KWalletTest : QApplication
{
public:
    KWalletTest (int& argc, char** argv) : QApplication (argc, argv) {
    }

    int exec() {

        KWalletStore* wallet = new KWalletStore;
        SecureStoreWidget test_widget (wallet);
        test_widget.show();
        return QApplication::exec();
    }
};

int main (int argc, char** argv)
{
    KWalletTest app (argc, argv);
    app.exec();
    return 0;
}
