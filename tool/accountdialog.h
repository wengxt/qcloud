#include <QDialog>

class QListView;
namespace Ui {
class AccountDialog;
}

namespace QCloud {

class AccountModel;
class AccountDialog : public QDialog {
    Q_OBJECT
public:
    explicit AccountDialog (QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AccountDialog();
    const QString& accountType();
private slots:
    void currentBackendChanged();
    void okClicked();
private:
    Ui::AccountDialog* m_ui;
    QCloud::AccountModel* m_backendModel;
    QString m_accountType;
};
}