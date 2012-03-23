#include <QCoreApplication>
#include "server.h"
namespace QCloud {
class Service : public Server
{
    Q_OBJECT
public:
    explicit Service (QObject* parent = 0);
    virtual ~Service();
};
}