#include <QApplication>
#include <QWidget>
#include <QDebug>
#include "dropbox.h"
#include "oauthwidget.h"
#include "factory.h"
#include "app.h"

int main (int argc, char* argv[])
{
    QApplication app (argc, argv);

    if (argc <= 4) {
        printf("Usage : download/upload [source] [destination]\n");
        return 0;
    }
    Dropbox* dropbox = new Dropbox;
    dropbox->setAppKey ("d2anwsztkcu1upz");
    dropbox->setAppSecret ("kt9a7tuph615hzs");
    dropbox->setNetworkAccessManager (QCloud::Factory::instance()->createNetwork ("kde"));
    QCloud::App a(argv[1]);

    dropbox->setApp(&a);

    bool result = dropbox->authorize();

    qDebug() << "Authorize result" << result;

    if (result) {
        dropbox->loadAccountInfo();
        if (strcmp(argv[2],"upload")==0){
            dropbox->uploadFile(argv[3],argv[4]);
        }
        else if (strcmp(argv[2],"download")==0){
            dropbox->downloadFile(argv[3],argv[4]);
        }
        else{
            qDebug() << "Invalid operation " << argv[2];
            return 1;
        }
    }
    return 0;
}
