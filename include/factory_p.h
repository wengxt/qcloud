#ifndef QCLOUD_FACTORY_P_H
#define QCLOUD_FACTORY_P_H

#include <QtCloud/Factory>
#include <QtCloud/IPlugin>

namespace QCloud
{

class IPlugin;

class FactoryPrivate
{
public:
    explicit FactoryPrivate (Factory* factory);
    virtual ~FactoryPrivate();

    void scan();
    void scan (const QString& category);
    IPlugin* loadPlugin (const QString& category, const QString& name);
    QMap<QString, IPlugin::PluginCategory> categoryMap;
    QMap<QString, QMap<QString, QPluginLoader* > > plugins;
    QList<IPlugin*> backendList;
    Factory* p;

    static Factory* inst;
};
}

#endif
