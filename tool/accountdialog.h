#include <QDialog>

class QDBusPendingCallWatcher;
class InfoModel;
class QListView;
namespace Ui
{
class AccountDialog;
}

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
    void appsFinished(QDBusPendingCallWatcher* watcher);
    void backendsFinished(QDBusPendingCallWatcher* watcher);
private:
    void fillData();
    Ui::AccountDialog* m_ui;
    InfoModel* m_backendModel;
    InfoModel* m_appModel;
    QString m_accountType;
    QString m_appName;
};
