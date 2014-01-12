#include "maindatabase.h"

MainDatabase::MainDatabase()
{
    instance = this;
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("C:/Users/Vijay13/Documents/Database/database.db");

    if(!mydb.open())
        qDebug("Could not open database");
    else
        qDebug("Opened database");

    this->Initialize();
}


MainDatabase* MainDatabase::instance = 0;

MainDatabase* MainDatabase::Create(){
    delete instance;
    instance = new MainDatabase();
    return instance;
}

MainDatabase* MainDatabase::Instance(){
    return instance;
}

void MainDatabase::Initialize(){
    QSqlQuery query;

    QString createQurey = "CREATE TABLE IF NOT EXISTS USERS(UID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, UserName NOT NULL UNIQUE, Password TEXT NOT NULL, isAdmin BOOL NOT NULL DEFAULT 0);";

    Q_ASSERT(query.exec(createQurey));
    Q_ASSERT(query.exec(getInsertUserQuery("Admin","Admin",true)));

    createQurey = "CREATE TABLE IF NOT EXISTS TALUKAS(TID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Taluka TEXT NOT NULL UNIQUE, District TEXT NOT NULL);";
    Q_ASSERT(query.exec(createQurey));
}

QString MainDatabase::getInsertUserQuery(QString user, QString pass, bool isAdmin){
    int admin=0;
    if(isAdmin){
        admin = 1;
    }
    return "INSERT OR IGNORE INTO USERS( UserName, Password, isAdmin) VALUES( '" + user + "', '" + pass +"', " + QString::number(admin) + ");";
}

QString MainDatabase::getInsertTalukaQuery(QString taluka, QString district){
    return "INSERT OR IGNORE INTO TALUKAS( Taluka, District) VALUES( '" + taluka + "', '" + district + "');";
}

QString MainDatabase::getUpdateTalukaQuery(int tid, QString newTaluka, QString newDistrict){
    return "UPDATE TALUKAS SET Taluka= '" + newTaluka + "', District = '" + newDistrict + "' WHERE TID = " + QString::number(tid) + " ;";
}

QString MainDatabase::getDeleteTalukaQuery(QString taluka, QString district){
    return "DELETE FROM TALUKAS WHERE Taluka = '" + taluka + "' AND District = '" + district + "' ;";
}
