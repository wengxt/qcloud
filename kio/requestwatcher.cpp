#include "requestwatcher.h"

RequestWatcher::RequestWatcher (int requestId, QObject* object) : QObject (object)
{
    m_requestId = requestId;
}

void RequestWatcher::receivedList (const QCloud::EntryInfoList& entryinfo, int requestId)
{
    if (requestId == m_requestId) {
        m_entryinfo = entryinfo;
        emit end();
    }
}