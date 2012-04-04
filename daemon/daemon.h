#ifndef DAEMON_H
#define DAEMON_H

#include <QCoreApplication>

class Service;
class Daemon : public QCoreApplication
{
    Q_OBJECT
public:
    Daemon (int& argc, char** argv);
    virtual ~Daemon();
    Service* m_service;
};

#endif // DAEMON_H