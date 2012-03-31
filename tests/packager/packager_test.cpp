#include <QDebug>

#include "packager.h"

int main (int argc, char* argv[])
{
    if (argc < 5)
        return 1;

    QCloud::Packager packager;
    qDebug() << "Compress File: " << argv[1];
    qDebug() << "Base Directory: " << argv[2];
    qDebug() << "Extract Directory: " << argv[3];
    qDebug() << "Files to compress:";

    QStringList list;
    for (int i = 4; i < argc; i ++) {
        qDebug() << argv[i];
        list << argv[i];
    }

    do {
        if (!packager.compress(argv[1], argv[2], list))
            break;
        qDebug() << "Extracting...";
        if (!packager.extract(argv[1], argv[3]))
            break;
        return 0;
    }
    while (0);
    return 1;
}
