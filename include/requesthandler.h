#ifndef QCLOUD_REQUEST_HANDLER_H
#define QCLOUD_REQUEST_HANDLER_H
#include <QObject>
#include "request.h"
#include "server.h"

namespace QCloud{

class QCLOUD_EXPORT RequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit RequestHandler(QObject* parent = 0);
    virtual ~RequestHandler();
    void setRequest(QCloud::Request* request);

signals:
    void uploadProgress (int m_id, qint64 sent, qint64 total);
    void downloadProgress (int m_id, qint64 sent, qint64 total);

public slots:
    virtual void requestFinished();

protected slots:
    void updateUploadProgress (qint64 sent, qint64 total);
    void updateDownloadProgress (qint64 sent, qint64 total);
protected:
    int m_id;
    Request* m_request;
    Server* m_server;
};
}
#endif