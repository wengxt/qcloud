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
private:
    Factory();
    void scan();
    void scan(const QString& category);
    QObject* loadPlugin(const QString& category, const QString& name);
    static Factory* inst;
    QStringList m_categoryList;
    
    QMap<QString, QMap<QString, QPluginLoader* > > m_plugins;
};
}