#ifndef QCLOUD_UTILS_H
#define QCLOUD_UTILS_H

#include <QtCore/QUrl>
#include "qcloud_global.h"
namespace QCloud
{
QCLOUD_EXPORT QUrl customCallbackUrl();
QCLOUD_EXPORT bool isCustomCallbackUrl (const QUrl& url);
}

#endif
