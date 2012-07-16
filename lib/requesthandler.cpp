#include <QDebug>

#include "requesthandler.h"

QCloud::RequestHandler::RequestHandler(QObject* parent): QObject(parent)
{
}

QCloud::RequestHandler::~RequestHandler()
{

}

void QCloud::RequestHandler::setRequest(QCloud::Request* request)
{
    m_request = request;
    request->setParent(this);
    connect(request,SIGNAL(finished()),this,SLOT(requestFinished()));
    connect(request,SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(updateUploadProgress(qint64, qint64)));
    connect(request,SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDownloadProgress(qint64, qint64)));
}

void QCloud::RequestHandler::requestFinished()
{

}

void QCloud::RequestHandler::updateUploadProgress(qint64 sent, qint64 total)
{
    qDebug() << "RequestHandler::updateUploadProgress" << m_id << sent << total;
    emit uploadProgress(m_id, sent, total);
}

void QCloud::RequestHandler::updateDownloadProgress(qint64 sent, qint64 total)
{
    qDebug() << "RequestHandler::updateDownloadProgress" << m_id << sent << total;
    emit downloadProgress(m_id, sent, total);
}
