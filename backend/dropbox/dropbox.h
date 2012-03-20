#ifndef DROPBOX_H
#define DROPBOX_H

#include "ibackend.h"

class Dropbox : public QCloud::IBackend
{
    Q_OBJECT
public:
    explicit Dropbox(QObject* parent = 0);
    virtual ~Dropbox();
    
    virtual bool customAouth(const QString & userName,const QString & password);
    virtual void authorize();
};

#endif