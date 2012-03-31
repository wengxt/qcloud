#ifndef QCLOUD_IPLUGIN_H
#define QCLOUD_IPLUGIN_H

#include <QtCore/QString>
#include <QtCore/QObject>
#include "qcloud_global.h"

namespace QCloud
{

class QCLOUD_EXPORT IPlugin
{
public:
    enum PluginCategory {
        Backend = 0,
        Network = 1,
        SecureStore = 2
    };
    virtual ~IPlugin();
    virtual QString name() = 0;
    virtual QString iconName();
    virtual QString displayName() = 0;
    virtual QObject* create() = 0;
    virtual PluginCategory category() = 0;
};

}

Q_DECLARE_INTERFACE (QCloud::IPlugin, "org.qcloud.plugin")

#endif
