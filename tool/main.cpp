#include <QApplication>
#include "mainwindow.h"
#include "clientapp.h"

int main (int argc, char* argv[])
{
    ClientApp app (argc, argv);

    return app.exec();
}
