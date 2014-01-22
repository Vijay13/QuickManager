#include "maindatabase.h"

#include <stdlib.h>
#include <QDir>
#include <QCoreApplication>

MainDatabase::MainDatabase()
{
    instance = this;
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    dbFileName = "/database.db";
    appDataFileName = "/data.txt";
    setAppDataPath();
}

void MainDatabase::Open(QString dirToDatabase)
{

    if(!dirToDatabase.isEmpty() && QDir(dirToDatabase).exists())
    {
        mydb.setDatabaseName(dirToDatabase + dbFileName);
        writeNewDBpath(dirToDatabase);
    }
    else if(!loadDBpath()->isEmpty())
    {
        for(int i = loadDBpath()->length() - 1; i >= 0; i--)
        {
            if(QDir(loadDBpath()->at(i)).exists())
            {
                mydb.setDatabaseName(loadDBpath()->at(i)+ dbFileName);
                break;
            }
        }
    }
    else
    {
        mydb.setDatabaseName(appDataPath + dbFileName);
    }

    if(!mydb.open())
        qDebug() << "Could not open database " + mydb.databaseName();
    else
        qDebug() << "Opened database " + mydb.databaseName();


    this->Initialize();
}

void MainDatabase::setAppDataPath()
{
    // this is where stdlib.h is being used
    appDataPath = getenv("LOCALAPPDATA");
    appDataPath.replace("\\","/");
    appDataPath += "/" + QCoreApplication::applicationName() + "/AppData";

    if(!QDir(appDataPath).exists())
    {
        if(!QDir().mkpath(appDataPath))
        {
            qDebug() << "Could not load appdata files";
        }
    }

    writeNewDBpath(appDataPath);
}

MainDatabase* MainDatabase::instance = 0;

MainDatabase* MainDatabase::Instance(){
    return instance;
}

void MainDatabase::Initialize(){
    QSqlQuery query;

    QString createQurey = "CREATE TABLE IF NOT EXISTS USERS(UID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, UserName NOT NULL UNIQUE, Password TEXT NOT NULL, isAdmin BOOL NOT NULL DEFAULT 0);";

    Q_ASSERT(query.exec(createQurey));

    if(query.exec(getInsertUserQuery("Admin","Admin",true))){
    }else{
        qDebug("Could not insert Admin");
    }

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

QList<QString>* MainDatabase::loadDBpath()
{
    QList<QString>* pathList = new QList<QString>();

    QFile file(appDataPath + appDataFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line = in.readLine();

    while(!line.isNull())
    {
        if(line.contains("#DATABASEPATH: "))
        {
            pathList->append(line.remove("#DATABASEPATH: "));
        }
        line = in.readLine();
    }
    file.close();
    return pathList;
}

void MainDatabase::writeNewDBpath(QString newPath)
{
    QFile file(appDataPath + appDataFileName);
    file.open(QIODevice::WriteOnly | QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString toWrite;

    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(!line.contains(newPath) && !line.isEmpty())
            toWrite.append(line + "\n");
    }

    file.resize(0);
    in << toWrite;
    in << "#DATABASEPATH: " << newPath << endl;
    file.close();
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

QString MainDatabase::getUpdateSchoolQuery(int SID, QString centerNo,QString taluka,
                                           QString schoolName,QString address,QString principal,
                                           QString principalMobNo,QString principalLandline,
                                           QString teacher,QString teacherMobno,QString principalAddress,
                                           QString routNo,QString girlMixboy,QString type,QString isprimary){
    QString toReturn;
    toReturn = "UPDATE SCHOOLS SET CenterNo = ";
    toReturn += getText(centerNo) + ",";
    toReturn += "Taluka = ";
    toReturn += getText(taluka)+ ", ";
    toReturn += "SchoolName = ";
    toReturn += getText(schoolName)+ ", ";
    toReturn += "Address = ";
    toReturn += getText(address)+ ", ";
    toReturn += "Principal = ";
    toReturn += getText(principal)+ ", ";
    toReturn += "PrincipalMobNo = ";
    toReturn += getText(principalMobNo) + ", ";
    toReturn += "PrincipalLandLine =";
    toReturn += getText(principalLandline)+ ", ";
    toReturn += "Teacher =";
    toReturn += getText(teacher)+ ", ";
    toReturn += "TeacherNo =";
    toReturn += getText(teacherMobno)+ ", ";
    toReturn += "PrincipalAddress =";
    toReturn += getText(principalAddress)+ ", ";
    toReturn += "RoutNo =";
    toReturn += getText(routNo)+ ", ";
    toReturn += "girlMIXboy =";
    toReturn += getText(girlMixboy) + ", ";
    toReturn += "Type =";
    toReturn += getText(type)+ ", ";
    toReturn += "isPrimary =";
    toReturn += getText(isprimary)+ " WHERE SID = " + QString::number(SID) + ";";

    return toReturn;
}

QString MainDatabase::getDeleteSchoolQuery(QString centerNo, QString taluka){
    return "DELETE FROM SCHOOLS WHERE Taluka = " + getText(taluka) + " AND CenterNo = " + getText(centerNo) + ";";
}

QString MainDatabase::getAllSchoolQuery(){
    return "SELECT * FROM SCHOOLS";
}

QString MainDatabase::getText(QString string){
    return "'" + string + "'";
}

QString MainDatabase::getCreateSchoolBillTable(QString tableName)
{
    return "CREATE TABLE IF NOT EXISTS " + tableName + " (" +
            "DATE INTEGER PRIMARY KEY NOT NULL, " +
            "SC_B INTEGER," + "SC_G INTEGER," + "ST_B INTEGER," + "ST_G INTEGER,"
            + "OBC_B INTEGER," + "OBC_G INTEGER," +
            "GEN_B INTEGER," + "GEN_G INTEGER," + "TOTAL_B INTEGER," + "TOTAL_G INTEGER,"
            + "TOTAL INTEGER" + ")";
}

QString MainDatabase::getInsertSchoolBill(QString table, int date, int sc_b, int sc_g,
                                          int st_b, int st_g, int obc_b,
                                          int obc_g, int gen_b, int gen_g,
                                          int total_b, int total_g, int total)
{
    return "INSERT OR REPLACE INTO " + table + "( DATE, SC_B, SC_G, ST_B, ST_G, OBC_B, OBC_G, GEN_B, GEN_G, TOTAL_B, TOTAL_G, TOTAL ) " +
            "VALUES ( " +
            QString::number(date) + ", " +
            QString::number(sc_b) + ", " +
            QString::number(sc_g) + ", " +
            QString::number(st_b) + ", " +
            QString::number(st_g) + ", " +
            QString::number(obc_b) + ", " +
            QString::number(obc_g) + ", " +
            QString::number(gen_b) + ", " +
            QString::number(gen_g) + ", " +
            QString::number(total_b) + ", " +
            QString::number(total_g) + ", " +
            QString::number(total) + " ) ";
}

QString MainDatabase::getSchoolBillTable(QString tableName)
{
    return "SELECT * FROM " + tableName;
}

QString MainDatabase::getDeleteSchoolBill(QString tableName)
{
    return "DROP TABLE IF EXISTS " + tableName;
}
