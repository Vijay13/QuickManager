#ifndef MAINDATABASE_H
#define MAINDATABASE_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class MainDatabase
{
public:
    MainDatabase();

    //initialises database
    void initialize();

    //returns pointer to main database file
    QSqlDatabase* getDatabase(){return &mydb;}

private:
    QSqlDatabase mydb;
};

#endif // MAINDATABASE_H
