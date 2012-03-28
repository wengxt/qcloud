#ifndef QCLOUD_FACTORY_P_H
#define QCLOUD_FACTORY_P_H

#include <QtCloud/Factory>
#include <QtCloud/IPlugin>

namespace QCloud {

class IPlugin;

class Factory::Private : public QObject {
    friend class Factory;
    Q_OBJECT
public:
    explicit Private (Factory* parent = 0);
    virtual ~Private();

private:
    void scan();
    void scan(const QString& category);
    IPlugin* loadPlugin(const QString& category, const QString& name);
    QMap<QString, IPlugin::PluginCategory> m_categoryMap;
    QMap<QString, QMap<QString, QPluginLoader* > > m_plugins;
    QList<IPlugin*> m_backendList;

    static Factory* inst;
};
}

#endif