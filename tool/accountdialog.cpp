#include "client.h"
#include "accountdialog.h"
#include "clientapp.h"
#include "infomodel.h"
#include "info.h"
#include "ui_accountdialog.h"

AccountDialog::AccountDialog (QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f),
    m_ui (new Ui::AccountDialog)
{
    m_ui->setupUi (this);

    m_appModel = new InfoModel (this);
    m_backendModel = new InfoModel (this);
    m_ui->backendView->setModel (m_backendModel);
    m_ui->backendView->setSelectionMode (QAbstractItemView::SingleSelection);
    m_ui->appView->setModel (m_appModel);
    m_ui->appView->setSelectionMode (QAbstractItemView::SingleSelection);

    connect (m_ui->backendView->selectionModel(), SIGNAL (currentChanged (QModelIndex, QModelIndex)), this, SLOT (currentBackendChanged()));
    connect (m_ui->appView->selectionModel(), SIGNAL (currentChanged (QModelIndex, QModelIndex)), this, SLOT (currentBackendChanged()));
    connect (m_ui->okButton, SIGNAL (clicked (bool)), this, SLOT (okClicked()));
    connect (m_ui->cancelButton, SIGNAL (clicked (bool)), this, SLOT (reject()));

    currentBackendChanged();
    fillData();
}

AccountDialog::~AccountDialog()
{
    delete m_ui;
}

void AccountDialog::okClicked()
{
    if (m_ui->backendView->currentIndex().isValid() && m_ui->appView->currentIndex().isValid()) {
        QModelIndex idx;
        idx = m_ui->backendView->currentIndex();
        m_accountType = static_cast<QCloud::Info*> (idx.internalPointer())->name();
        idx = m_ui->appView->currentIndex();
        m_appName = static_cast<QCloud::Info*> (idx.internalPointer())->name();
        accept();
    } else
        reject();
}

void AccountDialog::currentBackendChanged()
{
    if (m_ui->backendView->currentIndex().isValid() && m_ui->appView->currentIndex().isValid())
        m_ui->okButton->setEnabled (true);
    else
        m_ui->okButton->setEnabled (false);
}

void AccountDialog::fillData()
{
    QDBusPendingReply< QCloud::InfoList > apps = ClientApp::instance()->client()->listApps();
    QDBusPendingCallWatcher* appsWatcher = new QDBusPendingCallWatcher(apps);
    connect(appsWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(appsFinished(QDBusPendingCallWatcher*)));
    QDBusPendingReply< QCloud::InfoList > backends = ClientApp::instance()->client()->listBackends();
    QDBusPendingCallWatcher* backendsWatcher = new QDBusPendingCallWatcher(backends);
    connect(backendsWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(backendsFinished(QDBusPendingCallWatcher*)));
}

void AccountDialog::appsFinished (QDBusPendingCallWatcher* watcher)
{
    QDBusPendingReply< QCloud::InfoList > apps(*watcher);
    m_appModel->setInfoList(apps.value());
}

void AccountDialog::backendsFinished (QDBusPendingCallWatcher* watcher)
{
    QDBusPendingReply< QCloud::InfoList > backends(*watcher);
    m_backendModel->setInfoList(backends.value());
}


const QString& AccountDialog::accountType()
{
    return m_accountType;
}

const QString& AccountDialog::appName()
{
    return m_appName;
}
