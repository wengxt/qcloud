#include <QCoreApplication>
#include "daemon.h"

int main (int argc, char* argv[])
{
    Daemon app (argc, argv);

    return app.exec();
}
