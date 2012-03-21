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
    
    virtual QString GetItem(const QString& key);
    virtual bool SetItem(const QString& key, const QString& value);
 
};
 
}

#endif