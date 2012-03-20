#include <QWidget>
#include <QUrl>

class QPushButton;
class QWebView;
namespace Ui {
class AuthorizeWidget;
}

namespace QCloud {

class NetworkAccessManager;

class IBackend;

class Q_DECL_EXPORT AuthorizeWidget : public QWidget {
    Q_OBJECT
public:
    explicit AuthorizeWidget(IBackend* backend, QWidget* parent = 0);
    virtual ~AuthorizeWidget();
    void openUrl(const QUrl& url);
protected slots:
    void urlChanged(const QUrl& url);
    void authorizeButtonClicked();
    void authorizeSuccess();

protected:
    IBackend* m_backend;
    Ui::AuthorizeWidget* m_ui;
    QWebView* m_webView;
    QPushButton* m_authorizeButton;
    NetworkAccessManager* m_networkaccessmanager;

};

}