#include <QDialog>

class QListView;
namespace Ui {
class AccountDialog;
}

namespace QCloud {

class AppModel;

class BackendModel;
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
    QCloud::BackendModel* m_backendModel;
    QString m_accountType;
    QCloud::AppModel* m_appModel;
};
}