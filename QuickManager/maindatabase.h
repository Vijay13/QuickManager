#ifndef MAINDATABASE_H
#define MAINDATABASE_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class MainDatabase
{
public:
    MainDatabase();

    static MainDatabase* Instance();

    void Open(QString dirToDatabase);

    //initialises database
    void Initialize();

    void setAppDataPath();

    QList<QString>* loadDBpath();

    void writeNewDBpath(QString newPath);

    //returns pointer to main database file
    QSqlDatabase* getDatabase(){return &mydb;}

    QString getInsertUserQuery(QString user, QString pass, int isAdmin,
                               int TMAdd, int TMUpdate, int TMDelete,
                               int SMAdd, int SMUpdate, int SMDelete,
                               int SBMAdd, int SBMUpdate, int SBMDelete);

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

    QString getCreateSchoolBillTable(QString tableName);

    QString getInsertSchoolBill(QString table, int date, int sc_b, int sc_g,
                                int st_b, int st_g, int obc_b, int obc_g,
                                int gen_b, int gen_g, int total_b,
                                int total_g, int total,
                                int autoTotal_b, int autoTotal_g, int autoTotal,
                                int diffTotal_b, int diffTotal_g, int diffTotal);

    QString getSchoolBillTable(QString tableName);

    QString getDeleteSchoolBill(QString tableName);

    QString getCreateFoodListTable(QString tableName);

    QString getInsertFoodList(QString table, QString center, QString schoolName,
                              int std_1, int std_6, int roti_b, int roti_m, int roti_s,
                              int batch_no_roti, int rice_b, int rice_m, int rice_s,
                              int batch_no_rice, int subji_b, int subji_m, int subji_s,
                              int batch_no_subji, int total);

    QString getFoodListTable(QString tableName);

    QString getDeleteFoodListTable(QString tableName);

    QString getInsertErrorQuery(QString user,QString disc,QString status,QString date,QString model);

    QString getDeleteErrorQuery(QString disc,QString date);

    QString getAllErrorsQuery();

    QString getAppDataPath() const{return appDataPath;}

    QString getText(QString string);

private:
    QSqlDatabase mydb;
    static MainDatabase* instance;
    QString appDataPath;
    QString dbFileName, appDataFileName;
};

#endif // MAINDATABASE_H
