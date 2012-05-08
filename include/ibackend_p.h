#ifndef QCLOUD_IBACKEND_P_H
#define QCLOUD_IBACKEND_P_H

#include "ibackend.h"

namespace QCloud
{
class IBackendPrivate
{
public:
    IBackendPrivate ();
    virtual ~IBackendPrivate();

    Info info;
    QNetworkAccessManager* networkAccessManager;
    App* app;
};
}

#endif // QCLOUD_IBACKEND_P_H