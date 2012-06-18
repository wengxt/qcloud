#include "requesthandler.h"


void QCloud::RequestHandler::requestFinished()
{
    
}

QCloud::RequestHandler::RequestHandler(QObject* parent): QObject(parent)
{
    
}

void QCloud::RequestHandler::setRequest(QCloud::Request* request)
{
    m_request = request;
    connect(request,SIGNAL(finished()),this,SLOT(requestFinished()));
}

QCloud::RequestHandler::~RequestHandler()
{

}
