#ifndef QCLOUD_PLAINSTORE_H
#define QCLOUD_PLAINSTORE_H

#include "isecurestore.h"

class PlainStore : public QCloud::ISecureStore
{
    Q_OBJECT
public:
    PlainStore();
    virtual ~PlainStore();

    virtual bool isAvailable();

    bool GetItem (const QString& key, QString& value);
    virtual bool SetItem (const QString& key, const QString& value);

};

#endif
