#include <QtCore/QDebug>
#include <QNetworkAccessManager>

#include "ibackend.h"
#include "ibackend_p.h"

namespace QCloud
{


IBackendPrivate::IBackendPrivate () :
    networkAccessManager(0)
    ,app(0)
{

}

IBackendPrivate::~IBackendPrivate () {
}

IBackend::IBackend (QObject* parent) : QObject(parent)
    ,d(new IBackendPrivate)
{

}

IBackend::~IBackend()
{
    delete d;
}

void IBackend::setNetworkAccessManager (QNetworkAccessManager* manager)
{
    d->networkAccessManager = manager;
    manager->setParent(this);
}

QNetworkAccessManager* IBackend::networkAccessManager()
{
    return d->networkAccessManager;
}

App* IBackend::app()
{
    return d->app;
}

void IBackend::setApp(App* app)
{
    d->app = app;
}

void IBackend::setInfo(const Info& info)
{
    d->info = info;
}

const Info& IBackend::info()
{
    return d->info;
}

}
