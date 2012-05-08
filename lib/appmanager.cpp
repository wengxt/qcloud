#include <QCoreApplication>
#include <QDir>

#include "app.h"
#include "appmanager.h"
#include "appmanager_p.h"
#include "config.h"

namespace QCloud
{

AppManager* AppManagerPrivate::inst = NULL;

AppManagerPrivate::AppManagerPrivate ()
{
    scan();
}

AppManagerPrivate::~AppManagerPrivate ()
{
    foreach(App* app, appMap.values())
        delete app;
}

void AppManagerPrivate::scan()
{
    QString path = DATADIR;
    path.append ("/qcloud/app");
    QDir dir (path);
    if (!dir.exists())
        return ;
    QStringList entryList = dir.entryList();
    entryList.removeAll (".");
    entryList.removeAll ("..");
    if (entryList.isEmpty())
        return;

    foreach (const QString & maybeFile, entryList) {
        QFileInfo fi (dir.filePath (maybeFile));

        QString filePath = fi.filePath(); // file name with path
        App* app = new App (filePath);
        if (app->isValid())
            appMap[app->name()] = app;
        else
            delete app;
    }
}

AppManager::AppManager() : QObject()
    , d (new AppManagerPrivate)
{
}

AppManager::~AppManager()
{
    delete d;
}

App* AppManager::app (const QString& name)
{
    if (d->appMap.contains (name))
        return d->appMap[name];
    else
        return NULL;
}

QList<App*> AppManager::appList()
{
    return d->appMap.values();
}

AppManager* AppManager::instance()
{
    if (!AppManagerPrivate::inst)
        AppManagerPrivate::inst = new AppManager;
    return AppManagerPrivate::inst;
}

}
