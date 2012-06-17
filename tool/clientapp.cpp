#include "clientapp.h"
#include "mainwindow.h"
#include "app.h"
#include "client.h"

#include <QDebug>

ClientApp::ClientApp (int& argc, char** argv) : QApplication (argc, argv)
    ,m_client(0)
    ,m_mainWindow(new MainWindow)
{
    QCloud::Info::registerMetaType();
    m_client = new QCloud::Client(this);
    m_mainWindow->show();

    if (m_client->isValid())
        m_mainWindow->loadAccount();

    connect(m_client, SIGNAL(accountUpdated()), this, SLOT(accountUpdated()));
}

QCloud::Client* ClientApp::client()
{
    return m_client;
}

void ClientApp::accountUpdated()
{
    qDebug() << "accountUpdated";
    m_mainWindow->loadAccount();
}
