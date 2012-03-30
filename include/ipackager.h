#ifndef QCLOUD_IPACKAGE_H
#define QCLOUD_IPACKAGE_H

#include <archive.h>
#include <QObject>
#include <QString>
#include <QStringList>
#include "qcloud_global.h"

namespace QCloud{

class QCLOUD_EXPORT IPackager : public QObject
{
    Q_OBJECT;
public:
    IPackager();
    ~IPackager();
    bool CompressFile(const QString& FileName,const QStringList& FileList);
    bool ExtractFile(const QString& FileName,const QString& OutputPath);
};

}

#endif