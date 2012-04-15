#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDesktopServices>
#include <QSqlError>
#include <QSqlQuery>

#include "dbmanager.h"
#include "accountinfo.h"

DBManager* DBManager::m_instance = NULL;

DBManager* DBManager::instance()
{
    if (!m_instance) {
        m_instance = new DBManager;
    }
    return m_instance;
}

DBManager::DBManager (QObject* parent) : QObject (parent)
{
    QString xdgConfigHome = QFile::decodeName(qgetenv("XDG_CONFIG_HOME"));
    if (xdgConfigHome.isEmpty())
        xdgConfigHome = QDir::homePath() + QLatin1String("/.config");

    QDir dir(xdgConfigHome.append("/qcloud"));
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    m_dbpath = dir.filePath("qcloud.db");
    qDebug() << "Database Path: " << m_dbpath;
    if ( !QFile::exists( m_dbpath ) ) {
        if ( !createDB() ) {
            QMessageBox::critical( 0, tr( "Cannot create database" ),
                                   tr( m_db.lastError().text().toUtf8().data() ) );
            qDebug() << "Cannot create database, SQL error: " << m_db.lastError().text() << endl;
            exit ( 1 );
        }
    } else if ( !connectDB() )
        exit( 1 );
}

bool DBManager::createDB()
{
    bool ret = true;
    if ( !connectDB() )
        exit( 1 );
    QSqlQuery q;
    // account table
    if ( !q.exec( "CREATE TABLE account (id INTEGER PRIMARY KEY, app TEXT, backend TEXT, account_data TEXT)" ) ) {
        ret = false;
        qDebug() << q.lastError().text();
    }
    return ret;
}


bool DBManager::connectDB()
{
    if( m_db.isOpen() )
        return true;

    m_db = QSqlDatabase::addDatabase( "QSQLITE" );
    m_db.setDatabaseName( m_dbpath );

    if ( !m_db.open() ) {
        QMessageBox::critical( 0, tr( "Cannot connect to database" ),
                               tr( m_db.lastError().text().toUtf8().data() ) );

        qDebug() << "Cannot connect to database, SQL error: " << m_db.lastError().text();
        return false;
    }
    return true;

}

void DBManager::addAccount(const QCloud::AccountInfo& account)
{

}