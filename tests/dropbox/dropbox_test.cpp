#include <QApplication>
#include <QWidget>
#include <QDebug>
#include "dropbox.h"
#include "oauthwidget.h"
#include "factory.h"
#include "app.h"
#include "request.h"

void printUsage(char **argv)
{
    printf ("Usage : %s appfile download/upload/move/copy [source] [destination]\n",argv[0]);
    printf ("or    : %s appfile create_folder/delete/get_info [path]\n",argv[0]);
}

int main (int argc, char* argv[])
{
    QApplication app (argc, argv);

    if (argc <= 3) {
        printUsage(argv);
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
            if (argc <= 4) {
                printUsage(argv);
                return 0;
            }
            QCloud::Request* request = dropbox->uploadFile (QString::fromLocal8Bit(argv[3]), QString::fromLocal8Bit(argv[4]));
            request->waitForFinished();
        } else if (strcmp (argv[2], "download") == 0) {
            if (argc <= 4) {
                printUsage(argv);
                return 0;
            }
            QCloud::Request* request = dropbox->downloadFile (QString::fromLocal8Bit(argv[3]), QString::fromLocal8Bit(argv[4]));
            request->waitForFinished();
        } else if (strcmp(argv[2], "copy") == 0){
            if (argc <= 4) {
                printUsage(argv);
                return 0;
            }
            QCloud::Request* request = dropbox->copyFile (QString::fromLocal8Bit(argv[3]),QString::fromLocal8Bit(argv[4]));
            request->waitForFinished();
        }
        else if (strcmp(argv[2], "move") == 0){
            if (argc <= 4) {
                printUsage(argv);
                return 0;
            }
            QCloud::Request* request = dropbox->moveFile(QString::fromLocal8Bit(argv[3]),QString::fromLocal8Bit(argv[4]));
            request->waitForFinished();
        }
        else if (strcmp(argv[2], "create_folder") == 0){
            QCloud::Request* request = dropbox->createFolder(QString::fromLocal8Bit(argv[3]));
            request->waitForFinished();
        }
        else if (strcmp(argv[2], "delete") == 0){
            QCloud::Request* request = dropbox->deleteFile(QString::fromLocal8Bit(argv[3]));
            request->waitForFinished();
        }
        else if (strcmp(argv[2], "get_info") == 0){
            QVariantMap value;
            QCloud::Request* request = dropbox->pathInfo(QString::fromLocal8Bit(argv[3]),&value);
            request->waitForFinished();
        }
        else {
            qDebug() << "Invalid operation " << argv[2];
            return 1;
        }
    }
    return 0;
}
