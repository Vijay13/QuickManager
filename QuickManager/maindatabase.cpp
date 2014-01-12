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

    createQurey = "CREATE TABLE IF NOT EXISTS SCHOOLS (";
    createQurey += "SID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, ";
    createQurey += "CenterNo TEXT NOT NULL, ";
    createQurey += "Taluka TEXT, ";
    createQurey += "SchoolName TEXT NOT NULL UNIQUE, ";
    createQurey += "Address TEXT, ";
    createQurey += "Principal TEXT, ";
    createQurey += "PrincipalMobNo TEXT, ";
    createQurey += "PrincipalLandLine TEXT, ";
    createQurey += "Teacher TEXT, ";
    createQurey += "TeacherNo TEXT, ";
    createQurey += "PrincipalAddress TEXT, ";
    createQurey += "RoutNo TEXT, ";
    createQurey += "girlMIXboy TEXT, ";
    createQurey += "Type TEXT, ";
    createQurey += "isPrimary TEXT );";

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

QString MainDatabase::getAllTalukaQuery(){
    return "SELECT * FROM TALUKAS";
}

QString MainDatabase::getInsertSchoolQuery(QString centerNo,QString taluka,
                                           QString schoolName,QString address,QString principal,
                                           QString principalMobNo,QString principalLandline,
                                           QString teacher,QString teacherMobno,QString principalAddress,
                                           QString routNo,QString girlMixboy,QString type,QString isprimary){

    QString toReturn;
    toReturn = "INSERT OR IGNORE INTO SCHOOLS( ";
    toReturn += "CenterNo,Taluka,SchoolName,Address,";
    toReturn += "Principal,PrincipalMobNo,PrincipalLandLine,Teacher,TeacherNo,PrincipalAddress,RoutNo,";
    toReturn += "girlMIXboy,Type,isPrimary )";
    toReturn += " VALUES ";
    toReturn += "( " + getText(centerNo) + ", ";
    toReturn += getText(taluka)+ ", ";
    toReturn += getText(schoolName)+ ", ";
    toReturn += getText(address)+ ", ";
    toReturn += getText(principal)+ ", ";
    toReturn += getText(principalMobNo) + ", ";
    toReturn += getText(principalLandline)+ ", ";
    toReturn += getText(teacher)+ ", ";
    toReturn += getText(teacherMobno)+ ", ";
    toReturn += getText(principalAddress)+ ", ";
    toReturn += getText(routNo)+ ", ";
    toReturn += getText(girlMixboy) + ", ";
    toReturn += getText(type)+ ", ";
    toReturn += getText(isprimary)+ "); ";

    return toReturn;
}

QString MainDatabase::getDeleteSchoolQuery(QString centerNo, QString taluka){
    return "DELETE FROM SCHOOLS WHERE Taluka = '" + taluka + "' AND CenterNo = '" + centerNo + "' ;";
}

QString MainDatabase::getAllSchoolQuery(){
    return "SELECT * FROM SCHOOLS";
}

QString MainDatabase::getText(QString string){
    return "'" + string + "'";
}
