#include <QtCore/QDebug>

#include "ibackend.h"

namespace QCloud
{

IBackend::IBackend (QObject* parent) : QObject(parent)
    ,m_networkAccessManager(0)
    ,m_app(0)
{

}

IBackend::~IBackend()
{
}

void IBackend::setNetworkAccessManager (QNetworkAccessManager* manager)
{
    m_networkAccessManager = manager;
}

QNetworkAccessManager* IBackend::networkAccessManager()
{
    return m_networkAccessManager;
}

App* IBackend::app()
{
    return m_app;
}

void IBackend::setInfo(const Info& info)
{
    m_info = info;
}

const Info& IBackend::info()
{
    return m_info;
}

}
