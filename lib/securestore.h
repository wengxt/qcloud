#include <QObject>
#include <QString>


namespace QCloud
{

class ISecureStore : public QObject
{
    Q_OBJECT
public:

    virtual ~ISecureStore();

    virtual bool isAvailable() = 0;

    virtual QString GetItem(const QString& key) = 0;
    virtual bool SetItem(const QString& key, const QString& value) = 0;

};

}