#ifndef OAUTHBACKEND_P_H
#define OAUTHBACKEND_P_H

#include <QObject>
#include "oauthbackend.h"

namespace QCloud {
class OAuthBackendPrivate {
public:
    OAuthBackendPrivate(OAuthBackend* backend);
    virtual ~OAuthBackendPrivate();
    QByteArray oauthToken;
    QByteArray oauthTokenSecret;
    QString requestTokenUrl;
    QString accessTokenUrl;
    QUrl authUrl;
    QOAuth::Interface* oauth;
    QString appKey;
    QString appSecret;
};
}

#endif // OAUTHBACKEND_P_H