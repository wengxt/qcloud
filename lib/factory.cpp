#include <QNetworkAccessManager>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include "factory.h"
#include "ibackend.h"

#define PLUGIN_SUBDIR "qcloud"

namespace QCloud {
    
Factory* Factory::inst = NULL;
    
Factory::Factory()
{
    m_categoryList << "network" << "backend";
    scan();
}

void Factory::scan()
{
    Q_FOREACH(const QString& category, m_categoryList)
        scan(category);
}

void Factory::scan(const QString& category)
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


Factory* Factory::instance()
{
    if (!inst)
        inst = new Factory;
    return inst;
}


IBackend* Factory::loadBackendPlugin(const QString& name)
{
    return qobject_cast<IBackend*>(loadPlugin("backend", name));
}

QNetworkAccessManager* Factory::loadNetworkPlugin(const QString& name)
{
    return qobject_cast<QNetworkAccessManager*>(loadPlugin("network", name));
}

QObject* Factory::loadPlugin(const QString& category, const QString& name)
{
    QPluginLoader* loader = m_plugins[category][name];
    if (loader)
        return loader->instance();
        
    return NULL;
}


}