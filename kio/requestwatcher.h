#ifndef REQUESTWATCHER_H
#define REQUESTWATCHER_H
#include <QObject>
#include "entryinfo.h"

class RequestWatcher : public QObject {
    Q_OBJECT
public:
    RequestWatcher(int requestId, QObject* object = 0);

public slots:
    void receivedList(const QCloud::EntryInfoList& info, int requestId);
signals:
    void end();

public:

    QCloud::EntryInfoList m_entryinfo;
private:
    int m_requestId;
};

#endif