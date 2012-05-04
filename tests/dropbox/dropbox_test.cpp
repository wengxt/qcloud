#include <QApplication>
#include <QWidget>
#include <QDebug>
#include "dropbox.h"
#include "oauthwidget.h"
#include "factory.h"
#include "app.h"
#include "request.h"

int main (int argc, char* argv[])
{
    QApplication app (argc, argv);

    if (argc <= 4) {
        printf ("Usage : appfile download/upload [source] [destination]\n");
        return 0;
    }
    Dropbox* dropbox = new Dropbox;
    dropbox->setNetworkAccessManager (QCloud::Factory::instance()->createNetwork ("kde"));
    QCloud::App a (argv[1]);

    dropbox->setApp (&a);

    bool result = dropbox->authorize();

    qDebug() << "Authorize result" << result;

    if (result) {
        qDebug() << "User Name" << dropbox->userName();
        if (strcmp (argv[2], "upload") == 0) {
            QCloud::Request* request = dropbox->uploadFile (QString::fromLocal8Bit(argv[3]), QString::fromLocal8Bit(argv[4]));
            request->waitForFinished();
        } else if (strcmp (argv[2], "download") == 0) {
            QCloud::Request* request = dropbox->downloadFile (QString::fromLocal8Bit(argv[3]), QString::fromLocal8Bit(argv[4]));
            request->waitForFinished();
        } else if (strcmp(argv[2], "copy") == 0){
            qDebug() << "copy " << argv[3] << " to " << argv[4];
            QCloud::Request* request = dropbox->copyFile (QString::fromLocal8Bit(argv[3]),QString::fromLocal8Bit(argv[4]));
            request->waitForFinished();
        }
        else {
            qDebug() << "Invalid operation " << argv[2];
            return 1;
        }
    }
    return 0;
}
