#ifndef QMERROR_H
#define QMERROR_H

#include <QString>
#include "maindatabase.h"

/*
 * This class contains details of errors in data anywhere in database
 * which are required to be reported to manager
 * */
class QMError
{
public:
    int EID;
    QString user,disc,status,date,model;

    QMError();

    QMError(QString user,QString disc,QString status,QString date,QString model);

    QMError(int eid,QString user,QString disc,QString status,QString date,QString model);

private:
    MainDatabase *db;
    QSqlQuery *query;

    void Initialize();

    void setEID();

};

#endif // QMERROR_H
