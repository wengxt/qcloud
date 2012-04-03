#include <QDialog>

class QListView;
namespace Ui
{
class AccountDialog;
}

namespace QCloud
{

class AppModel;

class BackendModel;
class AccountDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AccountDialog (QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AccountDialog();
    const QString& accountType();
    const QString& appName();
private slots:
    void currentBackendChanged();
    void okClicked();
private:
    Ui::AccountDialog* m_ui;
    QCloud::BackendModel* m_backendModel;
    QCloud::AppModel* m_appModel;
    QString m_accountType;
    QString m_appName;
};
}
