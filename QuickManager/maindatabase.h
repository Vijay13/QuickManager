#ifndef MAINDATABASE_H
#define MAINDATABASE_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class MainDatabase
{
public:
    MainDatabase();

    static MainDatabase* Create();

    static MainDatabase* Instance();

    void Open(QString dirToDatabase);

    //initialises database
    void Initialize();

    //returns pointer to main database file
    QSqlDatabase* getDatabase(){return &mydb;}

    QString getInsertUserQuery(QString user, QString pass, bool isAdmin);

    QString getInsertTalukaQuery(QString taluka, QString district);

    QString getUpdateTalukaQuery(int tid, QString newTaluka, QString newDistrict);

    QString getDeleteTalukaQuery(QString taluka, QString district);

    QString getAllTalukaQuery();

    QString getInsertSchoolQuery(QString centerNo,QString taluka,
                                 QString schoolName,QString address,QString principal,
                                 QString principalMobNo,QString principalLandline,
                                 QString teacher,QString teacherMobno,QString principalAddress,
                                 QString routNo,QString girlMixboy,QString type,QString isprimary);

    QString getUpdateSchoolQuery(int SID, QString centerNo,QString taluka,
                                 QString schoolName,QString address,QString principal,
                                 QString principalMobNo,QString principalLandline,
                                 QString teacher,QString teacherMobno,QString principalAddress,
                                 QString routNo,QString girlMixboy,QString type,QString isprimary);

    QString getDeleteSchoolQuery(QString centerNo, QString taluka);

    QString getAllSchoolQuery();

    QString getText(QString string);

private:
    QSqlDatabase mydb;
    static MainDatabase* instance;
};

#endif // MAINDATABASE_H
