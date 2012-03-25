#ifndef QCLOUD_FACTORY_H
#define QCLOUD_FACTORY_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QStringList>

#include "qcloud_global.h"

class QPluginLoader;
class QNetworkAccessManager;
namespace QCloud {
class IBackend;

class QCLOUD_EXPORT Factory : public QObject {
    Q_OBJECT
public:
    static Factory* instance();
    QNetworkAccessManager* loadNetworkPlugin(const QString& name);
    IBackend* loadBackendPlugin(const QString& name);
    const QList< IBackend* >& backendList();
private:
    Factory();
    class Private;
    Private* d;
};
}

#endif // QCLOUD_FACTORY_H