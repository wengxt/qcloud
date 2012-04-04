#include "iplugin.h"

class KDENetworkPlugin: public QObject, public QCloud::IPlugin
{
    Q_OBJECT
    Q_INTERFACES (QCloud::IPlugin)
public:
    virtual QString name() const;
    virtual QString displayName() const;
    virtual QObject* create();
    virtual PluginCategory category();
};
