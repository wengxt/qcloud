#include "factory.h"
#include "ibackend.h"
#include "iplugin.h"
#include "accountdialog.h"
#include "ui_accountdialog.h"

namespace QCloud {

class AccountModel : public QAbstractListModel {
public:
    explicit AccountModel (QObject* parent = 0);
    virtual QModelIndex index (int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QList<IPlugin*> m_backendList;
};

AccountModel::AccountModel (QObject* parent) : QAbstractListModel (parent)
    , m_backendList(Factory::instance()->backendList())
{
}

QModelIndex AccountModel::index (int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column, (row >= 0 && row < m_backendList.count()? (void*) m_backendList.at(row) : 0 ));
}


QVariant AccountModel::data (const QModelIndex& index, int role) const
{
    IPlugin* backend = (IPlugin*) index.internalPointer();
    if (!backend)
        return QVariant();
    switch(role) {
        case Qt::DisplayRole:
            return backend->name();
        case Qt::DecorationRole:
            return QIcon::fromTheme(backend->iconName());
        default:
            return QVariant();
    }
}

int AccountModel::rowCount (const QModelIndex& parent) const
{
    return m_backendList.size();
}


AccountDialog::AccountDialog (QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f),
    m_ui(new Ui::AccountDialog)
{
    m_ui->setupUi(this);

    m_backendModel = new AccountModel(this);
    m_ui->backendView->setModel(m_backendModel);
    m_ui->backendView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_ui->backendView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(currentBackendChanged()));
    connect(m_ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
    connect(m_ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    currentBackendChanged();
}

AccountDialog::~AccountDialog()
{
    delete m_ui;
}

void AccountDialog::okClicked()
{
    if (m_ui->backendView->currentIndex().isValid()) {
        QModelIndex idx = m_ui->backendView->currentIndex();
        m_accountType = static_cast<IPlugin*>(idx.internalPointer())->name();
        accept();
    }
    else
        reject();
}

void AccountDialog::currentBackendChanged()
{
    if (m_ui->backendView->currentIndex().isValid())
        m_ui->okButton->setEnabled(true);
    else
        m_ui->okButton->setEnabled(false);
}



}