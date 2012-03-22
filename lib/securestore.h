#include <QObject>
#include <QString>


namespace QCloud
{

class ISecureStore : public QObject
{
    Q_OBJECT
public:

    virtual ~ISecureStore();

    bool isAvailable() = 0;

    QString GetItem(const QString& key) = 0;
    bool SetItem(const QString& key, const QString& value) = 0;

};

}