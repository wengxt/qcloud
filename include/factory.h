#ifndef QCLOUD_FACTORY_H
#define QCLOUD_FACTORY_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QStringList>

#include "qcloud_global.h"

class QPluginLoader;
class QNetworkAccessManager;
namespace QCloud
{

class ISecureStore;
class IPlugin;
class IBackend;
class FactoryPrivate;

class QCLOUD_EXPORT Factory : public QObject
{
    Q_OBJECT
public:
    static Factory* instance();
    QNetworkAccessManager* createNetwork (const QString& name, QObject* parent = 0);
    IBackend* createBackend (const QString& name, QObject* parent = 0);
    ISecureStore* createSecureStore (const QString& name, QObject* parent = 0);
    const QList< IPlugin* >& backendList();
private:
    Factory();
    virtual ~Factory();
    FactoryPrivate* d;
};
}

#endif // QCLOUD_FACTORY_H
