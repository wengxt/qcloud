#include <QNetworkReply>
namespace QCloud {


class Reply : public QNetworkReply {
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