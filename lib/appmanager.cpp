#include <QCoreApplication>
#include <QDir>

#include "app.h"
#include "appmanager.h"
#include "appmanager_p.h"
#include "config.h"

namespace QCloud
{

AppManager* AppManager::Private::inst = NULL;

AppManager::Private::Private (AppManager* parent) : QObject (parent)
{
    scan();
}

void AppManager::Private::scan()
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

    Q_FOREACH (const QString & maybeFile, entryList) {
        QFileInfo fi (dir.filePath (maybeFile));

        QString filePath = fi.filePath(); // file name with path
        App* app = new App (filePath);
        if (app->isValid())
            m_appMap[app->name()] = app;
        else
            delete app;
    }
}

AppManager::AppManager() : QObject()
    , d (new Private (this))
{

}

App* AppManager::app (const QString& name)
{
    if (d->m_appMap.contains (name))
        return d->m_appMap[name];
    else
        return NULL;
}

QList<App*> AppManager::appList()
{
    return d->m_appMap.values();
}

AppManager* AppManager::instance()
{
    if (!AppManager::Private::inst)
        AppManager::Private::inst = new AppManager;
    return AppManager::Private::inst;
}

}
