#include <QCoreApplication>
namespace QCloud {
class Daemon : public QCoreApplication
{
    Q_OBJECT
public:
    Daemon (int& argc, char** argv);
    virtual ~Daemon();
};
}