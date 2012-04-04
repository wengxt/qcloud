#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
class DBManager : public QObject
{
    Q_OBJECT
public:
    static DBManager* instance();
private:
    bool createDB();
    bool connectDB();

    static DBManager* m_instance;
    explicit DBManager (QObject* parent = 0);
    QSqlDatabase m_db;
    QString m_dbpath;
};

#endif // DBMANAGER_H