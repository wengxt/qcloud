#ifndef DROPBOX_H
#define DROPBOX_H

#include "oauthbackend.h"

class Dropbox : public QCloud::OAuthBackend
{
    Q_OBJECT
public:
    explicit Dropbox(QObject* parent = 0);
    virtual ~Dropbox();
    
    virtual void authorize();
};

#endif