#ifndef DAEMON_H
#define DAEMON_H

#include <QApplication>

class QNetworkAccessManager;
class Service;
class Daemon : public QApplication
{
    Q_OBJECT
public:
    Daemon (int& argc, char** argv);
    virtual ~Daemon();
private:
    Service* m_service;
    QNetworkAccessManager* m_networkAccessManager;
};

#endif // DAEMON_H