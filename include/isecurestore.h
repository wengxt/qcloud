#ifndef QCLOUD_ISECURESTORE_H
#define QCLOUD_ISECURESTORE_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include "qcloud_global.h"

namespace QCloud
{

class QCLOUD_EXPORT ISecureStore : public QObject
{
    Q_OBJECT
public:
    explicit ISecureStore (QObject* parent = 0);
    virtual ~ISecureStore();

    virtual bool isAvailable() = 0;

    virtual bool readItem (const QString& group, const QString& key, QByteArray& value) = 0;
    virtual bool writeItem (const QString& group, const QString& key, const QByteArray& value) = 0;
    virtual bool deleteItem (const QString& group, const QString& key) = 0;

};

}

#endif
