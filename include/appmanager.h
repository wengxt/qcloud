#ifndef QCLOUD_APPMANAGER_H
#define QCLOUD_APPMANAGER_H
#include <QtCore/QObject>
#include <QtCloud/App>
#include "qcloud_global.h"

namespace QCloud
{

class AppManagerPrivate;

class QCLOUD_EXPORT AppManager : public QObject
{
    Q_OBJECT
public:
    static AppManager* instance();
    QList< App* > appList();
    App* app (const QString& name);

private:
    explicit AppManager();
    virtual ~AppManager();
    AppManagerPrivate* d;
};
}

#endif
