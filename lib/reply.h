#ifndef QCLOUD_REPLY_H
#define QCLOUD_REPLY_H

#include <QtNetwork/QNetworkReply>
#include "qcloud_global.h"


namespace QCloud {

class QCLOUD_EXPORT Reply : public QNetworkReply {
    Q_OBJECT
public:
    Reply(const QUrl& url, QObject* parent = 0);
    
    virtual void abort();
    
    virtual qint64 bytesAvailable() const;
    
    virtual bool isSequential() const;
    
    virtual qint64 readData(char* data, qint64 maxlen);
private:
    QByteArray m_content;
    int m_offset;
};
}

#endif