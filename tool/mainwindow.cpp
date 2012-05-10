#include "mainwindow.h"
#include "accountdialog.h"
#include "clientapp.h"
#include "infomodel.h"
#include "factory.h"
#include "ibackend.h"
#include "client.h"
#include "appmanager.h"
#include "ui_tool.h"

#include <QDebug>

MainWindow::MainWindow (QWidget* parent, Qt::WindowFlags flags) : QMainWindow (parent, flags)
    , m_widget (new QWidget (this))
    , m_ui (new Ui::Tool)
    , m_accountModel (new InfoModel(this))
    , m_fileModel(new InfoModel(this))
{
    currentDir = "";
    
    setCentralWidget (m_widget);
    m_ui->setupUi (m_widget);
    m_ui->accountView->setModel(m_accountModel);
    m_ui->accountView->setSelectionMode(QAbstractItemView::SingleSelection);

    m_addAccountButton = m_ui->addAccountButton;
    m_deleteAccountButton = m_ui->deleteAccountButton;
    m_listButton = m_ui->listButton;

    m_addAccountButton->setIcon (QIcon::fromTheme ("list-add"));
    m_deleteAccountButton->setIcon (QIcon::fromTheme ("list-remove"));
    
    m_ui->fileView->setModel(m_fileModel);
    m_ui->fileView->setSelectionMode(QAbstractItemView::SingleSelection);

    setWindowTitle (tr ("QCloud"));
    setWindowIcon (QIcon::fromTheme ("qcloud"));

    connect (m_addAccountButton, SIGNAL (clicked (bool)), this, SLOT (addAccountButtonClicked()));
    connect (m_deleteAccountButton, SIGNAL(clicked(bool)), this, SLOT(deleteAccountButtonClicked()));
    connect (m_ui->fileView, SIGNAL(activated(QModelIndex)),this, SLOT(fileListActivated()));
    connect (m_listButton, SIGNAL(clicked(bool)),this, SLOT(listButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::addAccountButtonClicked()
{
    AccountDialog dialog;
    int result = dialog.exec();
    if (result == QDialog::Accepted && !dialog.accountType().isEmpty()) {
        ClientApp::instance()->client()->addAccount(dialog.accountType(), dialog.appName());
    }
}

void MainWindow::deleteAccountButtonClicked()
{
    if (!m_ui->accountView->currentIndex().isValid()){
        qDebug() << "Invalid index!";
        return ;
    }
    QModelIndex index;
    index = m_ui->accountView->currentIndex();
    QString uuid = static_cast<QCloud::Info*> (index.internalPointer())->name();
    if (uuid.isEmpty()){
        return ;
    }
    qDebug() << "Deteting account with ID : "<< uuid;
    qDebug() << "userName : " << static_cast<QCloud::Info*> (index.internalPointer())->displayName();
    ClientApp::instance()->client()->deleteAccount(uuid);
}

void MainWindow::accountsFinished(QDBusPendingCallWatcher* watcher)
{
    QDBusPendingReply< QCloud::InfoList > backends(*watcher);
    m_accountModel->setInfoList(backends.value());
}


void MainWindow::loadAccount()
{
    QDBusPendingReply< QCloud::InfoList > accounts = ClientApp::instance()->client()->listAccounts();
    QDBusPendingCallWatcher* appsWatcher = new QDBusPendingCallWatcher(accounts);
    connect(appsWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(accountsFinished(QDBusPendingCallWatcher*)));
}

bool MainWindow::loadFileList()
{
    qDebug() << "Loading file list...";
    if (!m_ui->accountView->currentIndex().isValid()){
        qDebug() << "Invalid index!";
        return false;
    }
    QModelIndex index;
    index = m_ui->accountView->currentIndex();
    QString uuid = static_cast< QCloud::Info* > (index.internalPointer())->name();
    if (uuid.isEmpty()){
        return false;
    }
    QDBusPendingReply< QCloud::InfoList > files = ClientApp::instance()->client()->listFiles(uuid,currentDir);
    QDBusPendingCallWatcher* appsWatcher = new QDBusPendingCallWatcher(files);
    connect(appsWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(filesFinished(QDBusPendingCallWatcher*)));
    return true;
}

void MainWindow::filesFinished(QDBusPendingCallWatcher* watcher)
{
    QDBusPendingReply< QCloud::InfoList > backends(*watcher);
    m_fileModel->setInfoList(backends.value());
}

void MainWindow::fileListActivated()
{    if (!m_ui->fileView->currentIndex().isValid()){
        qDebug() << "Invalid index!";
        return ;
    }
    QModelIndex index;
    index = m_ui->fileView->currentIndex();
    QString lastDir = currentDir;
    currentDir = static_cast<QCloud::Info*> (index.internalPointer())->name();
    QString is_dir = static_cast< QCloud::Info* > (index.internalPointer())->description();
    qDebug() << is_dir << " " << currentDir;
    if (is_dir!="is_dir" || (!loadFileList())){
        currentDir = lastDir;
        return ;
    }
}

void MainWindow::listButtonClicked()
{
    loadFileList();
}
