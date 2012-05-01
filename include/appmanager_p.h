#ifndef QCLOUD_APPMANAGER_P_H
#define QCLOUD_APPMANAGER_P_H
#include <QtCore/QMap>
#include <QtCloud/AppManager>

namespace QCloud
{
class AppManager::Private : public QObject
{
    friend class AppManager;
    Q_OBJECT
public:
    Private (AppManager* parent);

private:
    void scan();
    QMap<QString, App*> appMap;
    static AppManager* inst;
};
}

#endif
