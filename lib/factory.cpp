#include <QtCore/QPluginLoader>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>

#include "factory.h"
#include "factory_p.h"
#include "ibackend.h"
#include "isecurestore.h"
#include "iplugin.h"

#define PLUGIN_SUBDIR "qcloud"

namespace QCloud
{

Factory* FactoryPrivate::inst = NULL;

FactoryPrivate::FactoryPrivate (Factory* factory) : p(factory)
{
    categoryMap["network"] = IPlugin::Network;
    categoryMap["backend"] = IPlugin::Backend;
    categoryMap["securestore"] = IPlugin::SecureStore;
    scan();
}

FactoryPrivate::~FactoryPrivate()
{
}

void FactoryPrivate::scan()
{
    foreach (const QString & category, categoryMap.keys())
        scan (category);

    foreach (QPluginLoader * loader, plugins["backend"].values()) {
        IPlugin* backend = qobject_cast<IPlugin*> (loader->instance());
        backendList.push_back (backend);
    }
}

void FactoryPrivate::scan (const QString& category)
{
    // check plugin files
    const QStringList dirs = QCoreApplication::libraryPaths();
    if (dirs.isEmpty())
        return;
    for (QStringList::ConstIterator it = dirs.begin(); it != dirs.end(); ++it) {
        qDebug() << QString ("Checking Qt Library Path: %1").arg (*it);
        QDir libpath (*it);
        QDir dir (libpath.filePath (QString (PLUGIN_SUBDIR "/%1").arg (category)));
        if (!dir.exists()) {
            continue;
        }

        QStringList entryList = dir.entryList();
        // filter out "." and ".." to keep debug output cleaner
        entryList.removeAll (".");
        entryList.removeAll ("..");
        if (entryList.isEmpty()) {
            continue;
        }

        foreach (const QString & maybeFile, entryList) {
            QFileInfo fi (dir.filePath (maybeFile));

            QString filePath = fi.filePath(); // file name with path
            QString fileName = fi.fileName(); // just file name

            if (!QLibrary::isLibrary (filePath)) {
                continue;
            }

            QPluginLoader* loader = new QPluginLoader (filePath, p);
            IPlugin* plugin = qobject_cast< IPlugin* > (loader->instance());
            if (plugin) {
                if (plugin->category() == categoryMap[category])
                    plugins[category][plugin->name()] = loader;
                else
                    qDebug() << filePath << " is not a " << category  << "plugin!";

            }
        }
    }
}


IPlugin* FactoryPrivate::loadPlugin (const QString& category, const QString& name)
{
    QPluginLoader* loader = plugins[category][name];
    if (loader)
        return qobject_cast< IPlugin* > (loader->instance());

    return NULL;
}

Factory::Factory() : QObject (NULL)
    , d (new FactoryPrivate (this))
{
}

Factory::~Factory()
{
    delete d;
}

Factory* Factory::instance()
{
    if (!FactoryPrivate::inst)
        FactoryPrivate::inst = new Factory;
    return FactoryPrivate::inst;
}


IBackend* Factory::createBackend (const QString& name, QObject* parent)
{
    IPlugin* plugin = d->loadPlugin ("backend", name);
    if (plugin) {
        QObject* obj = plugin->create(parent);
        IBackend* backend = qobject_cast<IBackend*> (obj);

        do {
            if (obj && !backend) {
                delete obj;
            }

            backend->setInfo(*plugin);
            return backend;
        } while(0);
    }
    return NULL;
}

QNetworkAccessManager* Factory::createNetwork (const QString& name, QObject* parent)
{
    IPlugin* plugin = d->loadPlugin ("network", name);
    if (plugin)
        return qobject_cast<QNetworkAccessManager*> (plugin->create(parent));
    return NULL;
}

ISecureStore* Factory::createSecureStore (const QString& name, QObject* parent)
{
    IPlugin* plugin = d->loadPlugin ("securestore", name);
    if (plugin)
        return qobject_cast<ISecureStore*> (plugin->create(parent));
    return NULL;
}

const QList< IPlugin* >& Factory::backendList()
{
    return d->backendList;
}

}
