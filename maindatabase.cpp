#include "maindatabase.h"

MainDatabase::MainDatabase()
{
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("C:/Users/Vijay13/Documents/Database/database.db");

    if(!mydb.open())
        qDebug("Could not open database");
    else
        qDebug("Opened database");

    this->initialize();
}

void MainDatabase::initialize(){
    QSqlQuery query;

    QString createQurey = "CREATE TABLE IF NOT EXISTS USERS( UserName TEXT PRIMARY KEY NOT NULL, Password TEXT NOT NULL, isAdmin BOOL NOT NULL DEFAULT 0);";

    QString insertQurey = "INSERT OR IGNORE INTO USERS( UserName, Password, isAdmin) VALUES('Admin', 'Admin', 1);";

    Q_ASSERT(query.exec(createQurey));
    Q_ASSERT(query.exec(insertQurey));
}
