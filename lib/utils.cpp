#include "utils.h"

namespace QCloud
{

QUrl customCallbackUrl()
{
    return QUrl ("http://qcloud-custom-url");
}

bool isCustomCallbackUrl (const QUrl& url)
{
    return (url.scheme() == "http"  && url.host() == "qcloud-custom-url");
}

}
