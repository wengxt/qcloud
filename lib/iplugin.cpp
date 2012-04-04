#include "iplugin.h"

namespace QCloud
{
IPlugin::~IPlugin()
{

}

QString IPlugin::iconName() const
{
    return "";
}

QString IPlugin::description() const
{
    return "";
}

}
