#ifndef QCLOUD_PLAINSTORE_H
#define QCLOUD_PLAINSTORE_H

#include "isecurestore.h"

class PlainStore : public QCloud::ISecureStore
{
    Q_OBJECT
public:
    explicit PlainStore(QObject* parent);
    virtual ~PlainStore();

    virtual bool isAvailable();

    virtual bool readItem (const QString& group, const QString& key, QByteArray& value);
    virtual bool writeItem (const QString& group, const QString& key, const QByteArray& value);
    virtual bool deleteItem (const QString& group, const QString& key);

};

#endif
