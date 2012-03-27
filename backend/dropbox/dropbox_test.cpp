#include <QApplication>
#include <QWidget>
#include "dropbox.h"
#include "oauthwidget.h"
#include "factory.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Dropbox* dropbox = new Dropbox;
    dropbox->setAppKey("d2anwsztkcu1upz");
    dropbox->setAppSecret("kt9a7tuph615hzs");

    QCloud::OAuthWidget* widget = new QCloud::OAuthWidget(dropbox, QCloud::Factory::instance()->createNetwork("kde"));
    widget->show();

    app.exec();
    return 0;
}