#include <QtCore/QtPlugin>
#include <KAuthorized>
#include <KUrl>
#include <QDebug>
#include "kdenetworkaccessmanager.h"
#include "reply.h"
#include "utils.h"

namespace QCloud {
KDENetworkAccessManager::KDENetworkAccessManager(QObject* parent): KIO::Integration::AccessManager(parent)
{
    setObjectName("kde");
}

QNetworkReply* KDENetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* outgoingData)
{
    if (QCloud::isCustomCallbackUrl(request.url()) && op == QNetworkAccessManager::GetOperation)
        return new Reply(request.url(), this);
    
    return KIO::Integration::AccessManager::createRequest(op, request, outgoingData);
}
}

Q_EXPORT_PLUGIN2(network_kde, QCloud::KDENetworkAccessManager)