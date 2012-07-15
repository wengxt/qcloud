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

public slots:
    virtual void requestFinished();

protected:
    int m_id;
    Request* m_request;
    Server* m_server;
};
}
#endif