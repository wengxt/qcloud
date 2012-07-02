#include <QDebug>
#include "qcloud_utils.h"

#include "assert.h"
int main()
{
    QDateTime datetime = QCloud::parseRFC2822Date("Fri, 04 May 2012 06:42:47 +0000");
    qDebug() << datetime;
    assert(!datetime.isNull());
    datetime = QCloud::parseRFC2822Date("Wed, 25 Apr 2012 12:00:33 +0000");
    qDebug() << datetime;
    assert(!datetime.isNull());
    return 0;
}