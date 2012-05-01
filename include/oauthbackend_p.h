#ifndef OAUTHBACKEND_P_H
#define OAUTHBACKEND_P_H

#include <QObject>
#include "oauthbackend.h"

namespace QCloud {
class OAuthBackend::Private : public QObject {
    Q_OBJECT
public:
    Private(OAuthBackend* backend);
    virtual ~Private();
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