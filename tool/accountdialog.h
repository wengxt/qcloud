#include <QDialog>

class QComboBox;
namespace Ui {
class AccountDialog;
}

namespace QCloud {
class AccountDialog : public QDialog {
    Q_OBJECT
public:
    explicit AccountDialog (QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AccountDialog();
    const QString& accountType();
private:
    Ui::AccountDialog* m_ui;
    QComboBox* m_accountComboxBox;
};
}