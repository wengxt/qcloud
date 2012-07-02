#include "requestwatcher.h"

RequestWatcher::RequestWatcher (int requestId, QObject* object) : QObject (object)
{
    m_requestId = requestId;
}

void RequestWatcher::requestFinished (int id, uint error)
{
    if (id == m_requestId) {
        m_error = error;
        emit end();
    }
}


void RequestWatcher::reveivedInfo (int id, uint error, const QCloud::EntryInfo& info)
{
    if (id == m_requestId) {
        m_entryinfo = info;
        m_error = error;
        emit end();
    }
}

void RequestWatcher::receivedList (int id, uint error, const QCloud::EntryInfoList& info)
{
    if (id == m_requestId) {
        m_entryinfolist = info;
        m_error = error;
        emit end();
    }
}