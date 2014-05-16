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

#ifdef Q_OS_WIN
    // this is where stdlib.h is being used
    appDataPath = getenv("LOCALAPPDATA");
    appDataPath.replace("\\","/");
    appDataPath += "/" + QCoreApplication::applicationName() + "/AppData";
#endif

#ifdef Q_OS_LINUX
    appDataPath = QDir::currentPath();
#endif

    Q_ASSERT(!appDataPath.isEmpty());

    if(!QDir(appDataPath).exists())
    {
        if(!QDir().mkpath(appDataPath))
        {
            qDebug() << "Could not load appdata files";
        }
    }
}

MainDatabase* MainDatabase::instance = 0;

MainDatabase* MainDatabase::Instance(){
    return instance;
}

void MainDatabase::Initialize(){
    QSqlQuery query;

    QString createQurey = "CREATE TABLE IF NOT EXISTS USERS";
    createQurey += "( UID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,";
    createQurey += "UserName NOT NULL UNIQUE,";
    createQurey += "Password TEXT NOT NULL, ";
    createQurey += "isAdmin BOOL NOT NULL DEFAULT 0, ";
    createQurey += "TMAdd BOOL NOT NULL DEFAULT 1, ";
    createQurey += "TMUpdate BOOL NOT NULL DEFAULT 1, ";
    createQurey += "TMDelete BOOL NOT NULL DEFAULT 1, ";
    createQurey += "SMAdd BOOL NOT NULL DEFAULT 1, ";
    createQurey += "SMUpdate BOOL NOT NULL DEFAULT 1, ";
    createQurey += "SMDelete BOOL NOT NULL DEFAULT 1, ";
    createQurey += "SBMAdd BOOL NOT NULL DEFAULT 1, ";
    createQurey += "SBMUpdate BOOL NOT NULL DEFAULT 1, ";
    createQurey += "SBMDelete BOOL NOT NULL DEFAULT 1);";

    Q_ASSERT(query.exec(createQurey));

    if(query.exec(getInsertUserQuery("Admin","Admin",1,1,1,1,1,1,1,1,1,1)))
    {
    }
    else
    {
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

    createQurey = "CREATE TABLE IF NOT EXISTS ERRORS(EID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, USER TEXT, DISC TEXT, STATUS TEXT, DATE TEXT, MODEL TEXT);";
    Q_ASSERT(query.exec(createQurey));

}

QList<QString>* MainDatabase::loadDBpath()
{
    QList<QString>* pathList = new QList<QString>();

    QFile file(appDataPath + appDataFileName);
    file.open(QIODevice::WriteOnly |QIODevice::ReadOnly | QIODevice::Text);
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

QString MainDatabase::getInsertUserQuery(QString user, QString pass, int isAdmin,
                                         int TMAdd, int TMUpdate, int TMDelete,
                                         int SMAdd, int SMUpdate, int SMDelete,
                                         int SBMAdd, int SBMUpdate, int SBMDelete){
    return "INSERT OR IGNORE INTO USERS( UserName, Password, isAdmin , TMAdd, TMUpdate, TMDelete, SMAdd, SMUpdate, SMDelete, SBMAdd, SBMUpdate, SBMDelete) VALUES( "
            + getText(user) + ", "
            + getText(pass) +", "
            + QString::number(isAdmin) + ","
            + QString::number(TMAdd) + ","
            + QString::number(TMUpdate) + ","
            + QString::number(TMDelete) + ","
            + QString::number(SMAdd) + ","
            + QString::number(SMUpdate) + ","
            + QString::number(SMDelete) + ","
            + QString::number(SBMAdd) + ","
            + QString::number(SBMUpdate) + ","
            + QString::number(SBMDelete) + ");";
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

QString MainDatabase::getCreateSchoolBillTable(QString tableName)
{
    return "CREATE TABLE IF NOT EXISTS " + tableName + " (" +
            "DATE INTEGER PRIMARY KEY NOT NULL, " +
            "SC_B INTEGER," + "SC_G INTEGER," + "ST_B INTEGER," + "ST_G INTEGER,"
            + "OBC_B INTEGER," + "OBC_G INTEGER," +
            "GEN_B INTEGER," + "GEN_G INTEGER," + "TOTAL_B INTEGER," + "TOTAL_G INTEGER,"
            + "TOTAL INTEGER," + "AUTOTOTAL_B INTEGER,"
            + "AUTOTOTAL_G INTEGER," + "AUTOTOTAL INTEGER,"
            + "DIFFTOTAL_B INTEGER," + "DIFFTOTAL_G INTEGER,"
            + "DIFFTOTAL INTEGER"
            + ")";
}

QString MainDatabase::getInsertSchoolBill(QString table, int date, int sc_b, int sc_g,
                                          int st_b, int st_g, int obc_b,
                                          int obc_g, int gen_b, int gen_g,
                                          int total_b, int total_g, int total,
                                          int autoTotal_b, int autoTotal_g, int autoTotal,
                                          int diffTotal_b, int diffTotal_g, int diffTotal)
{
    return "INSERT OR REPLACE INTO " + table + "( DATE, SC_B, SC_G, ST_B, ST_G, OBC_B, OBC_G, GEN_B, GEN_G, TOTAL_B, TOTAL_G, TOTAL, AUTOTOTAL_B, AUTOTOTAL_G, AUTOTOTAL, DIFFTOTAL_B, DIFFTOTAL_G, DIFFTOTAL ) " +
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
            QString::number(total) + ", " +
            QString::number(autoTotal_b) + ", " +
            QString::number(autoTotal_g) + ", " +
            QString::number(autoTotal) + ", " +
            QString::number(diffTotal_b) + ", " +
            QString::number(diffTotal_g) + ", " +
            QString::number(diffTotal) + " ) ";
}

QString MainDatabase::getSchoolBillTable(QString tableName)
{
    return "SELECT * FROM " + tableName ;
}

QString MainDatabase::getDeleteSchoolBill(QString tableName)
{
    return "DROP TABLE IF EXISTS " + tableName;
}

QString MainDatabase::getCreateFoodListTable(QString tableName){
    return "CREATE TABLE IF NOT EXISTS " + tableName + " (" +
            "CENTER TEXT PRIMARY KEY NOT NULL, " +
            "SCHOOL TEXT, " + "STD_1 INTEGER, " + "STD_6 INTEGER, " +
            "ROTI_B INTEGER, " + "ROTI_M INTEGER, " + "ROTI_S INTEGER, " +
            "ROTI_BATCH_NO INTEGER, " + "RICE_B INTEGER, " + "RICE_M INTEGER, " + "RICE_S INTEGER, " +
            "RICE_BATCH_NO INTEGER, " + "SUBJI_B INTEGER, " + "SUBJI_M INTEGER, " + "SUBJI_S INTEGER, " +
            "SUBJI_BATCH_NO INTEGER, " + "TOTAL INTEGER" + ")";
}

QString MainDatabase::getInsertFoodList(QString table, QString center, QString schoolName,
                                        int std_1, int std_6, int roti_b, int roti_m, int roti_s,
                                        int batch_no_roti, int rice_b, int rice_m, int rice_s,
                                        int batch_no_rice, int subji_b, int subji_m, int subji_s,
                                        int batch_no_subji, int total){

    return "INSERT OR REPLACE INTO " + table + "( CENTER, SCHOOL, STD_1, STD_6, ROTI_B, ROTI_M, ROTI_S, ROTI_BATCH_NO, RICE_B, RICE_M, RICE_S, RICE_BATCH_NO, SUBJI_B, SUBJI_M, SUBJI_S, SUBJI_BATCH_NO, TOTAL ) " +
            "VALUES ( " +
            getText(center) + ", " +
            getText(schoolName) + ", " +
            QString::number(std_1) + ", " +
            QString::number(std_6) + ", " +
            QString::number(roti_b) + ", " +
            QString::number(roti_m) + ", " +
            QString::number(roti_s) + ", " +
            QString::number(batch_no_roti) + ", " +
            QString::number(rice_b) + ", " +
            QString::number(rice_m) + ", " +
            QString::number(rice_s) + ", " +
            QString::number(batch_no_rice) + ", " +
            QString::number(subji_b) + ", " +
            QString::number(subji_m) + ", " +
            QString::number(subji_s) + ", " +
            QString::number(batch_no_subji) + ", " +
            QString::number(total) + ") ";

}

QString MainDatabase::getFoodListTable(QString tableName){
    return "SELECT * FROM " + tableName ;
}

QString MainDatabase::getDeleteFoodListTable(QString tableName){
    return "DROP TABLE IF EXISTS " + tableName ;
}

QString MainDatabase::getInsertErrorQuery(QString user, QString disc, QString status, QString date, QString model)
{
    return "INSERT INTO ERRORS (USER,DISC,STATUS,DATE,MODEL) VALUES ( " + getText(user) + ","
            + getText(disc) + "," + getText(status) + "," + getText(date) + "," + getText(model) + " )";
}

QString MainDatabase::getDeleteErrorQuery(QString disc,QString date)
{
    return "DELETE FROM ERRORS WHERE DESC = " + getText(disc) + "AND DATE = " + getText(date);
}

QString MainDatabase::getAllErrorsQuery()
{
    return "SELECT * FROM ERRORS";
}

QString MainDatabase::getText(QString string){
    return "'" + string + "'";
}
