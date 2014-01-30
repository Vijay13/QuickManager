#ifndef ALLERROR_H
#define ALLERROR_H

#include "DataModel/qmerror.h"

class AllError
{
public:
    AllError();

    static AllError* Instance();

    void Initialize();

    QList<QMError*>* getErrorList(){return errorList;}

    void addError(QString user,QString disc,QString status,QString date,QString model);

    int getErrorPos(QString disc,QString date);

    QMError* getError(QString disc,QString date);

    void removeError(QString disc,QString date);

private:
    static AllError* instance;
    QList<QMError*>* errorList;
    MainDatabase *db;
    QSqlQuery *query;
};

#endif // ALLERROR_H
