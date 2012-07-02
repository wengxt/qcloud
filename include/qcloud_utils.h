#ifndef QCLOUD_UTILS_H
#define QCLOUD_UTILS_H

#include <QtCore/QUrl>
#include <QtCore/QDateTime>
#include "qcloud_global.h"
namespace QCloud
{
QCLOUD_EXPORT QUrl customCallbackUrl();
QCLOUD_EXPORT bool isCustomCallbackUrl (const QUrl& url);
QCLOUD_EXPORT QDateTime parseRFC2822Date(const QString& string);
}

#endif
