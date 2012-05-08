#ifndef QCLOUD_APPMANAGER_P_H
#define QCLOUD_APPMANAGER_P_H
#include <QtCore/QMap>
#include <QtCloud/AppManager>

namespace QCloud
{
class AppManagerPrivate
{
    friend class AppManager;
public:
    AppManagerPrivate ();
    virtual ~AppManagerPrivate();

private:
    void scan();
    QMap<QString, App*> appMap;
    static AppManager* inst;
};
}

#endif
