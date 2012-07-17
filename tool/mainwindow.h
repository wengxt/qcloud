#include <QMainWindow>
#include <QSet>
#include "infomodel.h"
#include "entryinfomodel.h"
#include "entryinfo.h"

class QDBusPendingCallWatcher;
class InfoModel;
class QNetworkAccessManager;
class QPushButton;
namespace Ui
{
class Tool;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow (QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~MainWindow();

    void loadAccount();
    bool loadFileList();

private slots:
    void accountsFinished(QDBusPendingCallWatcher* watcher);
    void addAccountButtonClicked();
    void deleteAccountButtonClicked();
    void fileListActivated();
    void listButtonClicked();
    void createFolderTriggered();
    void deleteFileTriggered();
    void requestFinished(int requestId,uint error);
    void fileListFinished(int id, uint error, const QCloud::EntryInfoList& info);
private:
    QString getUuid();
    void removeId(int id);
    
    QWidget* m_widget;
    Ui::Tool* m_ui;
    InfoModel* m_accountModel;
    EntryInfoModel* m_fileModel;
    QPushButton* m_addAccountButton;
    QPushButton* m_deleteAccountButton;
    QPushButton* m_listButton;
    QString currentDir;
    QSet<int> idSet;
    QMap<int,QString> idPath;
    QAction* refreshAction;
    QAction* createFolderAction;
    QAction* deleteFileAction;
    QAction* downloadAction;
    QAction* uploadAction;
};
