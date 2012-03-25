#include <QtCore/QDebug>

#include "ibackend.h"

namespace QCloud {

IBackend::IBackend(QObject* parent)
{
}

IBackend::~IBackend()
{

}

const QString& IBackend::name()
{
    return m_name;
}

const QString& IBackend::iconName()
{
    return m_iconName;
}

bool IBackend::prepare()
{
    return true;
}

void IBackend::setNetworkAccessManager(QNetworkAccessManager* manager)
{
}

void IBackend::setAuthorizeWidget(AuthorizeWidget* widget)
{
    m_authWidget = widget;
}



}