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

    if (argc <= 2) {
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
        dropbox->uploadFile(argv[2], "test_file");
    }
    return 0;
}
