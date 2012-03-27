#include "mainwindow.h"
#include "accountdialog.h"
#include "factory.h"
#include "ibackend.h"
#include "ui_tool.h"

namespace QCloud {
MainWindow::MainWindow (QWidget* parent, Qt::WindowFlags flags) : QMainWindow (parent, flags)
    ,m_widget(new QWidget(this))
    ,m_ui(new Ui::Tool)
{
    setCentralWidget(m_widget);
    m_ui->setupUi(m_widget);

    m_addAccountButton = m_ui->addAccountButton;
    m_deleteAccountButton = m_ui->deleteAccountButton;

    m_addAccountButton->setIcon(QIcon::fromTheme("list-add"));
    m_deleteAccountButton->setIcon(QIcon::fromTheme("list-remove"));

    setWindowTitle(tr("QCloud"));
    setWindowIcon(QIcon::fromTheme("qcloud"));

    connect(m_addAccountButton, SIGNAL(clicked(bool)), this, SLOT(addAccountButtonClicked()));
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
        IBackend* account = QCloud::Factory::instance()->createBackend(dialog.accountType());
        if (account) {
            account->authorize();
        }
    }
}

void MainWindow::deleteAccountButtonClicked()
{

}


}