#include <QtCore/QPluginLoader>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>

#include "factory.h"
#include "factory_p.h"
#include "ibackend.h"
#include "iplugin.h"

#define PLUGIN_SUBDIR "qcloud"

namespace QCloud {

Factory* Factory::Private::inst = NULL;

Factory::Private::Private(Factory* factory) : QObject(factory) {
    m_categoryMap["network"] = IPlugin::Network;
    m_categoryMap["backend"] = IPlugin::Backend;
    m_categoryMap["securestore"] = IPlugin::SecureStore;
    scan();
}

Factory::Private::~Private() {
}

void Factory::Private::scan()
{
    Q_FOREACH(const QString& category, m_categoryMap.keys())
        scan(category);

    Q_FOREACH(QPluginLoader* loader, m_plugins["backend"].values())
    {
        IPlugin* backend = qobject_cast<IPlugin*>(loader->instance());
        m_backendList.push_back(backend);
    }
}

void Factory::Private::scan(const QString& category)
{
    // check plugin files
    const QStringList dirs = QCoreApplication::libraryPaths();
    if(dirs.isEmpty())
        return;
    for(QStringList::ConstIterator it = dirs.begin(); it != dirs.end(); ++it)
    {
        qDebug() << QString("Checking Qt Library Path: %1").arg(*it);
        QDir libpath(*it);
        QDir dir(libpath.filePath(QString(PLUGIN_SUBDIR "/%1").arg(category)));
        if(!dir.exists())
        {
            continue;
        }

        QStringList entryList = dir.entryList();
        // filter out "." and ".." to keep debug output cleaner
        entryList.removeAll(".");
        entryList.removeAll("..");
        if(entryList.isEmpty())
        {
            continue;
        }

        Q_FOREACH(const QString &maybeFile, entryList)
        {
            QFileInfo fi(dir.filePath(maybeFile));

            QString filePath = fi.filePath(); // file name with path
            QString fileName = fi.fileName(); // just file name

            if(!QLibrary::isLibrary(filePath))
            {
                continue;
            }

            QPluginLoader* loader = new QPluginLoader(filePath, this);
            IPlugin* plugin = qobject_cast< IPlugin* >(loader->instance());
            if (plugin) {
                if (plugin->category() == m_categoryMap[category])
                    m_plugins[category][plugin->name()] = loader;
                else
                    qDebug() << filePath << " is not a " << category  << "plugin!";

            }
        }
    }
}


IPlugin* Factory::Private::loadPlugin(const QString& category, const QString& name)
{
    QPluginLoader* loader = m_plugins[category][name];
    if (loader)
        return qobject_cast< IPlugin* >(loader->instance());

    return NULL;
}

Factory::Factory() : QObject(NULL)
    ,d(new Private(this))
{
}

Factory* Factory::instance()
{
    if (!Private::inst)
        Private::inst = new Factory;
    return Private::inst;
}


IBackend* Factory::createBackend(const QString& name)
{
    IPlugin* plugin = d->loadPlugin("backend", name);
    if (plugin)
        return qobject_cast<IBackend*>(plugin->create());
    return NULL;
}

QNetworkAccessManager* Factory::createNetwork(const QString& name)
{
    IPlugin* plugin = d->loadPlugin("network", name);
    if (plugin)
        return qobject_cast<QNetworkAccessManager*>(plugin->create());
    return NULL;
}

const QList< IPlugin* >& Factory::backendList()
{
    return d->m_backendList;
}

}