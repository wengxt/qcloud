#include <QtCore/QDebug>

#include "ibackend.h"

namespace QCloud
{

IBackend::IBackend (QObject* parent)
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


}
