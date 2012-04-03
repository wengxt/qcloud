#include "factory.h"
#include "ibackend.h"
#include "iplugin.h"
#include "app.h"
#include "appmanager.h"
#include "accountdialog.h"
#include "ui_accountdialog.h"

namespace QCloud
{


class AppModel : public QAbstractListModel
{
public:
    explicit AppModel (QObject* parent = 0);
    virtual QModelIndex index (int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QList<App*> m_appList;
};

AppModel::AppModel (QObject* parent) : QAbstractListModel (parent)
    , m_appList (AppManager::instance()->appList())
{
}

QModelIndex AppModel::index (int row, int column, const QModelIndex& parent) const
{
    return createIndex (row, column, (row >= 0 && row < m_appList.count() ? (void*) m_appList.at (row) : 0));
}


QVariant AppModel::data (const QModelIndex& index, int role) const
{
    App* app = (App*) index.internalPointer();
    if (!app)
        return QVariant();
    switch (role) {
    case Qt::DisplayRole:
        return app->name();
    case Qt::DecorationRole:
        return QIcon::fromTheme (app->iconName());
    default:
        return QVariant();
    }
}

int AppModel::rowCount (const QModelIndex& parent) const
{
    return m_appList.size();
}

class BackendModel : public QAbstractListModel
{
public:
    explicit BackendModel (QObject* parent = 0);
    virtual QModelIndex index (int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QList<IPlugin*> m_backendList;
};

BackendModel::BackendModel (QObject* parent) : QAbstractListModel (parent)
    , m_backendList (Factory::instance()->backendList())
{
}

QModelIndex BackendModel::index (int row, int column, const QModelIndex& parent) const
{
    return createIndex (row, column, (row >= 0 && row < m_backendList.count() ? (void*) m_backendList.at (row) : 0));
}


QVariant BackendModel::data (const QModelIndex& index, int role) const
{
    IPlugin* backend = (IPlugin*) index.internalPointer();
    if (!backend)
        return QVariant();
    switch (role) {
    case Qt::DisplayRole:
        return backend->name();
    case Qt::DecorationRole:
        return QIcon::fromTheme (backend->iconName());
    default:
        return QVariant();
    }
}

int BackendModel::rowCount (const QModelIndex& parent) const
{
    return m_backendList.size();
}


AccountDialog::AccountDialog (QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f),
    m_ui (new Ui::AccountDialog)
{
    m_ui->setupUi (this);

    m_appModel = new AppModel (this);
    m_backendModel = new BackendModel (this);
    m_ui->backendView->setModel (m_backendModel);
    m_ui->backendView->setSelectionMode (QAbstractItemView::SingleSelection);
    m_ui->appView->setModel (m_appModel);
    m_ui->appView->setSelectionMode (QAbstractItemView::SingleSelection);

    connect (m_ui->backendView->selectionModel(), SIGNAL (currentChanged (QModelIndex, QModelIndex)), this, SLOT (currentBackendChanged()));
    connect (m_ui->appView->selectionModel(), SIGNAL (currentChanged (QModelIndex, QModelIndex)), this, SLOT (currentBackendChanged()));
    connect (m_ui->okButton, SIGNAL (clicked (bool)), this, SLOT (okClicked()));
    connect (m_ui->cancelButton, SIGNAL (clicked (bool)), this, SLOT (reject()));

    currentBackendChanged();
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
        m_accountType = static_cast<IPlugin*> (idx.internalPointer())->name();
        idx = m_ui->appView->currentIndex();
        m_appName = static_cast<App*> (idx.internalPointer())->name();
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

const QString& AccountDialog::accountType()
{
    return m_accountType;
}

const QString& AccountDialog::appName()
{
    return m_appName;
}

}
