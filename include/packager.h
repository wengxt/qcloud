#ifndef QCLOUD_PACKAGE_H
#define QCLOUD_PACKAGE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "qcloud_global.h"

namespace QCloud
{

class QCLOUD_EXPORT Packager : public QObject
{
    Q_OBJECT;
public:
    Packager();
    virtual ~Packager();
    bool compress (const QString& fileName, const QString& baseDir, const QStringList& fileList, bool ignoreError = true);
    bool extract (const QString& fileName, const QString& outputPath);
};

}

#endif // QCLOUD_PACKAGE_H
