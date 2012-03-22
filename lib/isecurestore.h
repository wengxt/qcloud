#ifndef QCLOUD_ISECURESTORE_H
#define QCLOUD_ISECURESTORE_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include "qcloud_global.h"

namespace QCloud {

class QCLOUD_EXPORT ISecureStore : public QObject
{
Q_OBJECT
public:
   virtual ~ISecureStore();

   virtual bool isAvailable() = 0;

   virtual bool GetItem(const QString& key, QString& value) = 0;
   virtual bool SetItem(const QString& key, const QString& value) = 0;

};

}

#endif