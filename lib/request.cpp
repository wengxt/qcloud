#include <QEventLoop>
#include "request.h"

namespace QCloud
{
Request::Request(QObject* parent) : QObject(parent) {
}

void Request::waitForFinished()
{
    QEventLoop loop;
    QObject::connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

Request::~Request()
{
}
}