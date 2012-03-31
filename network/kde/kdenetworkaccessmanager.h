#include <KIO/AccessManager>

class KDENetworkAccessManager : public KIO::Integration::AccessManager
{
    Q_OBJECT
public:
    explicit KDENetworkAccessManager (QObject* parent = 0);

    virtual QNetworkReply* createRequest (Operation op, const QNetworkRequest& request, QIODevice* outgoingData = 0);
};
