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

class QCLOUD_EXPORT Factory : public QObject
{
    Q_OBJECT
public:
    static Factory* instance();
    QNetworkAccessManager* createNetwork (const QString& name);
    IBackend* createBackend (const QString& name);
    ISecureStore* createSecureStore (const QString& name);
    const QList< IPlugin* >& backendList();
private:
    Factory();
    class Private;
    Private* d;
};
}

#endif // QCLOUD_FACTORY_H
