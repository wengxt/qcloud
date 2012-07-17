#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>

#include "mainwindow.h"
#include "accountdialog.h"
#include "clientapp.h"
#include "infomodel.h"
#include "factory.h"
#include "ibackend.h"
#include "client.h"
#include <qdbusconnection.h>
#include "appmanager.h"
#include "ui_tool.h"


MainWindow::MainWindow (QWidget* parent, Qt::WindowFlags flags) : QMainWindow (parent, flags)
    , m_widget (new QWidget (this))
    , m_ui (new Ui::Tool)
    , m_accountModel (new InfoModel(this))
    , m_fileModel(new EntryInfoModel(this))
{
    currentDir = "/";

    setCentralWidget (m_widget);
    m_ui->setupUi (m_widget);
    m_ui->accountView->setModel(m_accountModel);
    m_ui->accountView->setSelectionMode(QAbstractItemView::SingleSelection);

    m_addAccountButton = m_ui->addAccountButton;
    m_deleteAccountButton = m_ui->deleteAccountButton;
    m_listButton = m_ui->listButton;

    m_addAccountButton->setIcon (QIcon::fromTheme ("list-add"));
    m_deleteAccountButton->setIcon (QIcon::fromTheme ("list-remove"));
    
    refreshAction = new QAction("Refresh",this);
    createFolderAction = new QAction("Create Folder",this);
    downloadAction = new QAction("Download",this);
    uploadAction = new QAction("Upload",this);
    deleteFileAction = new QAction("Delete",this);

    m_ui->fileView->setModel(m_fileModel);
    m_ui->fileView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ui->fileView->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_ui->fileView->addAction(refreshAction);
    m_ui->fileView->addAction(createFolderAction);
    m_ui->fileView->addAction(downloadAction);
    m_ui->fileView->addAction(uploadAction);
    m_ui->fileView->addAction(deleteFileAction);

    setWindowTitle (tr ("QCloud"));
    setWindowIcon (QIcon::fromTheme ("qcloud"));

    connect (m_addAccountButton, SIGNAL (clicked (bool)), this, SLOT (addAccountButtonClicked()));
    connect (m_deleteAccountButton, SIGNAL(clicked(bool)), this, SLOT(deleteAccountButtonClicked()));
    connect (m_ui->fileView, SIGNAL(activated(QModelIndex)),this, SLOT(fileListActivated()));
    connect (m_listButton, SIGNAL(clicked(bool)),this, SLOT(listButtonClicked()));
    connect (m_ui->createFolderButton, SIGNAL(clicked(bool)),this, SLOT(createFolderTriggered()));
    connect (refreshAction,SIGNAL(triggered(bool)),this,SLOT(listButtonClicked()));
    connect (createFolderAction,SIGNAL(triggered(bool)),this,SLOT(createFolderTriggered()));
    connect (m_ui->accountView,SIGNAL(clicked(QModelIndex)),this,SLOT(listButtonClicked()));
    connect (deleteFileAction,SIGNAL(triggered(bool)),this,SLOT(deleteFileTriggered()));
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
    delete watcher;
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
    QString uuid = getUuid();
    if (uuid.isEmpty()){
        return false;
    }
    QDBusPendingReply< int > id = ClientApp::instance()->client()->listFiles(uuid,currentDir);
    //ClientApp::instance()->client()->listFiles(uuid,currentDir);
    QEventLoop* loop = new QEventLoop;
    QDBusPendingCallWatcher* appsWatcher = new QDBusPendingCallWatcher(id);
    connect(appsWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)), loop,SLOT(quit()));
    loop->exec();
    delete loop;
    idSet.insert(id.value());
    idPath[id.value()] = currentDir;
    delete appsWatcher;
    //connect(ClientApp::instance()->client(),SIGNAL(directoryInfoTransformed(QCloud::InfoList)),this,SLOT(fileListFinished(QCloud::InfoList)));
    connect(ClientApp::instance()->client(),SIGNAL(directoryInfoTransformed(int,uint,QCloud::EntryInfoList)),this,SLOT(fileListFinished(int,uint,QCloud::EntryInfoList)));
    return true;
}

void MainWindow::fileListActivated()
{    if (!m_ui->fileView->currentIndex().isValid()){
        qDebug() << "Invalid index!";
        return ;
    }
    QModelIndex index;
    index = m_ui->fileView->currentIndex();
    QString lastDir = currentDir;
    currentDir = static_cast<QCloud::EntryInfo*> (index.internalPointer())->path();
    bool is_dir = static_cast< QCloud::EntryInfo* > (index.internalPointer())->isDir();
    qDebug() << is_dir << " " << currentDir;
    if (!is_dir || (!loadFileList())){
        currentDir = lastDir;
        return ;
    }
}

void MainWindow::listButtonClicked()
{
    loadFileList();
}

void MainWindow::fileListFinished(int id, uint error, const QCloud::EntryInfoList& info)
{
    if (!idSet.contains(id))
        return ;
    if (error!=0){
        QMessageBox msgBox;
        QString text = QString("Error while listing directory ,id=%1 code=%2").arg(id,error);
        qDebug() << text;
        msgBox.setText(text);
        msgBox.exec();
        return ;
    }
    qDebug() << "Got file list info with ID : " << id;
    QFileInfo currentDirInfo(idPath[id]);
    QCloud::EntryInfo parentPath;
    parentPath.setValue(QCloud::EntryInfo::PathType,QVariant(currentDirInfo.path()));
    parentPath.setValue(QCloud::EntryInfo::DirType,QVariant(true));
    QCloud::EntryInfoList m_info = info;
    if (currentDirInfo.absolutePath()!="" && currentDirInfo.absolutePath()!=currentDirInfo.absoluteFilePath())
        m_info << parentPath;
    m_fileModel->setEntryInfoList(idPath[id],m_info);
    removeId(id);
}

QString MainWindow::getUuid()
{
    QModelIndex index;
    index = m_ui->accountView->currentIndex();
    return static_cast< QCloud::Info* > (index.internalPointer())->name();
}

void MainWindow::createFolderTriggered()
{
    bool ok;
    QString dirName = QInputDialog::getText(this,"Directory Name","Please Input Directory Name",QLineEdit::Normal,"",&ok);
    if (!ok)
        return ;
    QDir parentPath(currentDir);
    QString path = parentPath.absoluteFilePath(dirName);
    QString uuid = getUuid();
    if (uuid.isEmpty())
        return ;
    QDBusPendingReply < int > id = ClientApp::instance()->client()->createFolder(uuid,path);
    QDBusPendingCallWatcher *appsWatcher = new QDBusPendingCallWatcher(id);
    QEventLoop* loop = new QEventLoop;
    connect(appsWatcher,SIGNAL(finished(QDBusPendingCallWatcher*)),loop, SLOT(quit()));
    loop->exec();
    delete loop;
    delete appsWatcher;
    idSet.insert(id.value());
    idPath[id.value()] = path;
    connect(ClientApp::instance()->client(),SIGNAL(requestFinished(int,uint)),this,SLOT(requestFinished(int,uint)));
}

void MainWindow::requestFinished(int requestId, uint error)
{
    if (!idSet.contains(requestId))
        return ;
    if (error!=0){
        QMessageBox msgBox;
        QString text = QString("Error while processing ,id=%1 code=%2").arg(requestId).arg(error);
        qDebug() << text;
        msgBox.setText(text);
        msgBox.exec();
        return ;
    }
    loadFileList();
    removeId(requestId);
}

void MainWindow::removeId(int id)
{
    idSet.remove(id);
    idPath.remove(id);
}

void MainWindow::deleteFileTriggered()
{
    if (!m_ui->fileView->currentIndex().isValid()){
        qDebug() << "Invalid index!";
        return ;
    }
    QModelIndex index;
    index = m_ui->fileView->currentIndex();
    QString currentPath = static_cast<QCloud::EntryInfo*> (index.internalPointer())->path();
    QMessageBox msgBox;
    msgBox.setText(QString("Are you sure to remove file '%1'?").arg(currentPath));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = msgBox.exec();
    if (ret==QMessageBox::No)
        return ;
    
    QString uuid = getUuid();
    if (uuid.isEmpty())
        return ;
    QDBusPendingReply < int > id = ClientApp::instance()->client()->deleteFile(uuid,currentPath);
    QDBusPendingCallWatcher *appsWatcher = new QDBusPendingCallWatcher(id);
    QEventLoop* loop = new QEventLoop;
    connect(appsWatcher,SIGNAL(finished(QDBusPendingCallWatcher*)),loop, SLOT(quit()));
    loop->exec();
    delete loop;
    delete appsWatcher;
    idSet.insert(id);
    idPath[id] = currentPath;
    connect(ClientApp::instance()->client(),SIGNAL(requestFinished(int,uint)),this,SLOT(requestFinished(int,uint)));
}

