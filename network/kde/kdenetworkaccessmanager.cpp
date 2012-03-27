#include <QtCore/QtPlugin>
#include <KAuthorized>
#include <KUrl>
#include <QDebug>
#include "kdenetworkaccessmanager.h"
#include "reply.h"
#include "utils.h"

KDENetworkAccessManager::KDENetworkAccessManager(QObject* parent): KIO::Integration::AccessManager(parent)
{
}

QNetworkReply* KDENetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* outgoingData)
{
    if (QCloud::isCustomCallbackUrl(request.url()) && op == QNetworkAccessManager::GetOperation)
        return new QCloud::Reply(request.url(), this);

    return KIO::Integration::AccessManager::createRequest(op, request, outgoingData);
}