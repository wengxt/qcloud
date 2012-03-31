#include <QtCore/QtPlugin>
#include <QNetworkAccessManager>

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit NetworkAccessManager (QObject* parent = 0);

    virtual QNetworkReply* createRequest (Operation op, const QNetworkRequest& request, QIODevice* outgoingData = 0);
};
