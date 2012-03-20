#include "networkaccessmanager.h"
#include "reply.h"
#include <QNetworkRequest>
#include <QNetworkProxy>

namespace QCloud {

NetworkAccessManager::NetworkAccessManager(QNetworkAccessManager* oldManager, QObject* parent): QNetworkAccessManager(parent)
{
    m_oldManager = oldManager;
    setCache(oldManager->cache());
    setCookieJar(oldManager->cookieJar());
    setProxy(oldManager->proxy());
    setProxyFactory(oldManager->proxyFactory());
}

QNetworkReply* NetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* outgoingData)
{
    if (request.url().scheme() != "qcloud")
        return QNetworkAccessManager::createRequest(op, request, outgoingData);
    
    if (op == QNetworkAccessManager::GetOperation)
        return new Reply(request.url(), this);
    
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}

}