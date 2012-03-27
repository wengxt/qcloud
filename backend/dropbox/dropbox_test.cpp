#include <QApplication>
#include <QWidget>
#include <QDebug>
#include "dropbox.h"
#include "oauthwidget.h"
#include "factory.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Dropbox* dropbox = new Dropbox;
    dropbox->setAppKey("d2anwsztkcu1upz");
    dropbox->setAppSecret("kt9a7tuph615hzs");
    dropbox->setNetworkAccessManager(QCloud::Factory::instance()->createNetwork("kde"));

    qDebug() << "Authorize result" << dropbox->authorize();
    return 0;
}