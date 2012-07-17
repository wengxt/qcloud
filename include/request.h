#ifndef QCLOUD_REQUEST_H
#define QCLOUD_REQUEST_H
#include <QObject>
#include <qcloud_global.h>

namespace QCloud
{
class QCLOUD_EXPORT Request : public QObject
{
    Q_OBJECT
public:
    enum Error {
        NoError,
        AuthorizeError,
        NetworkError,
        FileError,
        FileExistsError,
        FileNotFoundError,
        OtherError
    };

    explicit Request (QObject* parent = 0);
    virtual ~Request();
    virtual Error error() = 0;

    void waitForFinished();

signals:
    void finished();
    void uploadProgress(qint64 send, qint64 total);
    void downloadProgress(qint64 send, qint64 total);
};
}

#endif
