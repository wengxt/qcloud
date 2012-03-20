#include <QTimer>
#include <QDebug>
#include "reply.h"

namespace QCloud {

Reply::Reply(const QUrl& url, QObject* parent): QNetworkReply(parent)
    , m_content("<html><head><title>Test</title></head><body>This is a test.</body></html>")
    , m_offset(0)
{
    setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/html; charset=ASCII"));
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(m_content.length()));
    QTimer::singleShot(0, this, SIGNAL(readyRead()));
    QTimer::singleShot(0, this, SIGNAL(finished()));
    open(ReadOnly | Unbuffered);
    setUrl(url);
    qDebug() << url;
}

void Reply::abort()
{
}

qint64 Reply::bytesAvailable() const
{
    return m_content.length() - m_offset;
}

bool Reply::isSequential() const
{
    return true;
}

qint64 Reply::readData(char* data, qint64 maxlen)
{
    if (m_offset < m_content.length()) {
        qint64 end = qMin(m_offset + maxlen, (qint64) m_content.length());
        int len = end - m_offset;
        memcpy(data, m_content.mid(m_offset, len).data(), len);
        m_offset = end;
        return len;
    }
    return -1;
}

}