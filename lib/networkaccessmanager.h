#include <QNetworkAccessManager>
namespace QCloud {
class NetworkAccessManager : public QNetworkAccessManager
{
public:
    explicit NetworkAccessManager(QNetworkAccessManager* oldManager, QObject* parent = 0);
    
    virtual QNetworkReply* createRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData = 0);
private:
    QNetworkAccessManager* m_oldManager;
};
}