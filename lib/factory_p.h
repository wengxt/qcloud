#ifndef QCLOUD_FACTORY_P_H
#define QCLOUD_FACTORY_P_H

#include "factory.h"

namespace QCloud {

class Factory::Private : public QObject {
    friend class Factory;
    Q_OBJECT
public:
    explicit Private (Factory* parent = 0);
    virtual ~Private();

private:
    void scan();
    void scan(const QString& category);
    QObject* loadPlugin(const QString& category, const QString& name);
    QStringList m_categoryList;
    QMap<QString, QMap<QString, QPluginLoader* > > m_plugins;
    QList<IBackend*> m_backendList;

    static Factory* inst;
};
}

#endif