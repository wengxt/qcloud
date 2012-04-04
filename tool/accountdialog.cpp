#include "client.h"
#include "accountdialog.h"
#include "clientapp.h"
#include "info.h"
#include "ui_accountdialog.h"

class InfoModel : public QAbstractListModel
{
public:
    explicit InfoModel (QObject* parent = 0);
    virtual QModelIndex index (int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    void setInfoList(QCloud::InfoList infoList);
private:
    QCloud::InfoList m_infoList;
};

InfoModel::InfoModel (QObject* parent) : QAbstractListModel (parent)
{
}

QModelIndex InfoModel::index (int row, int column, const QModelIndex& parent) const
{
    return createIndex (row, column, (row >= 0 && row < m_infoList.count() ? (void*) &m_infoList.at (row) : 0));
}


QVariant InfoModel::data (const QModelIndex& index, int role) const
{
    const QCloud::Info* app = (QCloud::Info*) index.internalPointer();
    if (!app)
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return app->displayName();
    case Qt::DecorationRole:
        return QIcon::fromTheme (app->iconName());
    default:
        return QVariant();
    }
}

int InfoModel::rowCount (const QModelIndex& parent) const
{
    return m_infoList.size();
}

void InfoModel::setInfoList (QCloud::InfoList infoList)
{
    beginRemoveRows(QModelIndex(), 0, m_infoList.size());
    m_infoList.clear();
    endRemoveRows();
    beginInsertRows(QModelIndex(), 0, infoList.count() - 1);
    m_infoList = infoList;
    endInsertRows();
}


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
