#include <QtCore/QDebug>

#include "ibackend.h"

namespace QCloud {

IBackend::IBackend(QObject* parent)
{
}

IBackend::~IBackend()
{

}

bool IBackend::prepare()
{
    return true;
}

void IBackend::setNetworkAccessManager(QNetworkAccessManager* manager)
{
}

void IBackend::setAuthorizeWidget(OAuthWidget* widget)
{
    m_authWidget = widget;
}



}