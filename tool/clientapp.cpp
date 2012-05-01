#include "clientapp.h"
#include "mainwindow.h"
#include "app.h"
#include "client.h"

ClientApp::ClientApp (int& argc, char** argv) : QApplication (argc, argv)
    ,m_client(new QCloud::Client(this))
    ,m_mainWindow(new MainWindow)
{
    QCloud::Info::registerMetaType();
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
    m_mainWindow->loadAccount();
}
