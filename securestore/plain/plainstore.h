#ifndef QCLOUD_PLAINSTORE_H
#define QCLOUD_PLAINSTORE_H

#include "isecurestore.h"

namespace QCloud {

class PlainStore : public ISecureStore
{
Q_OBJECT
public:
    PlainStore();
    virtual ~PlainStore();

    virtual bool isAvailable();

    bool GetItem(const QString& key, QString& value);
    virtual bool SetItem(const QString& key, const QString& value);

};

}

#endif
