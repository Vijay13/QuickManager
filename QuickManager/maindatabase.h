#ifndef MAINDATABASE_H
#define MAINDATABASE_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class MainDatabase
{
public:
    MainDatabase();

    static MainDatabase* Create();

    static MainDatabase* Instance();

    //initialises database
    void Initialize();

    //returns pointer to main database file
    QSqlDatabase* getDatabase(){return &mydb;}

    QString getInsertUserQuery(QString user, QString pass, bool isAdmin);

    QString getInsertTalukaQuery(QString taluka, QString district);

    QString getUpdateTalukaQuery(int tid, QString newTaluka, QString newDistrict);

    QString getDeleteTalukaQuery(QString taluka, QString district);

private:
    QSqlDatabase mydb;
    static MainDatabase* instance;
};

#endif // MAINDATABASE_H
