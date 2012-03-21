#include <QNetworkRequest>

#include "networkaccessmanager.h"
#include "reply.h"
#include "utils.h"

namespace QCloud {

NetworkAccessManager::NetworkAccessManager(QObject* parent): QNetworkAccessManager(parent)
{
    setObjectName("general");
}

QNetworkReply* NetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* outgoingData)
{
    if (QCloud::isCustomCallbackUrl(request.url()) && op == QNetworkAccessManager::GetOperation)
        return new Reply(request.url(), this);
    
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}

}

Q_EXPORT_PLUGIN2(network_kde, QCloud::NetworkAccessManager)