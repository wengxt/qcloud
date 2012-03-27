#include <QNetworkRequest>

#include "networkaccessmanager.h"
#include "reply.h"
#include "utils.h"

NetworkAccessManager::NetworkAccessManager(QObject* parent): QNetworkAccessManager(parent)
{
}

QNetworkReply* NetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* outgoingData)
{
    if (QCloud::isCustomCallbackUrl(request.url()) && op == QNetworkAccessManager::GetOperation)
        return new QCloud::Reply(request.url(), this);

    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}