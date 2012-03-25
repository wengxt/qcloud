#include <QtCore/QPluginLoader>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>

#include "factory.h"
#include "factory_p.h"
#include "ibackend.h"

#define PLUGIN_SUBDIR "qcloud"

namespace QCloud {

Factory* Factory::Private::inst = NULL;

Factory::Private::Private(Factory* factory) : QObject(factory) {
    m_categoryList << "network" << "backend";
    scan();
}

Factory::Private::~Private() {
}

void Factory::Private::scan()
{
    Q_FOREACH(const QString& category, m_categoryList)
        scan(category);

    Q_FOREACH(QPluginLoader* loader, m_plugins["backend"].values())
    {
        IBackend* backend = qobject_cast<IBackend*>(loader->instance());
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
            QObject* object = loader->instance();

            m_plugins[category][object->objectName()] = loader;
        }
    }
}


QObject* Factory::Private::loadPlugin(const QString& category, const QString& name)
{
    QPluginLoader* loader = m_plugins[category][name];
    if (loader)
        return loader->instance();

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


IBackend* Factory::loadBackendPlugin(const QString& name)
{
    return qobject_cast<IBackend*>(d->loadPlugin("backend", name));
}

QNetworkAccessManager* Factory::loadNetworkPlugin(const QString& name)
{
    return qobject_cast<QNetworkAccessManager*>(d->loadPlugin("network", name));
}

const QList< IBackend* >& Factory::backendList()
{
    return d->m_backendList;
}

}