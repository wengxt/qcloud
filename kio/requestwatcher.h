#ifndef REQUESTWATCHER_H
#define REQUESTWATCHER_H
#include <QObject>
#include "entryinfo.h"

class RequestWatcher : public QObject {
    Q_OBJECT
public:
    RequestWatcher(int requestId, QObject* object = 0);

public slots:
    void requestFinished(int id, uint error);
    void reveivedInfo (int id, uint error, const QCloud::EntryInfo& info);
    void receivedList(int id, uint error, const QCloud::EntryInfoList& info);
signals:
    void end();

public:

    QCloud::EntryInfoList m_entryinfolist;
    QCloud::EntryInfo m_entryinfo;
    uint m_error;
private:
    int m_requestId;
};

#endif