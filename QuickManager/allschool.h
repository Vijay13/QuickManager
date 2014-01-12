#ifndef ALLSCHOOL_H
#define ALLSCHOOL_H

#include <school.h>

class AllSchool
{
public:
    AllSchool();

    static AllSchool* Instance();

    void Initialize();

    void addSchool(School* school);

    void removeSchool(QString centerNo, QString taluka);

    void removeFromList(QString centerNo, QString taluka);

    QList<School*>* getSchoolList(){return schoolList;}

private:
    static AllSchool* instance;
    QList<School*>* schoolList;
    MainDatabase *db;
    QSqlQuery *query;
};

#endif // ALLSCHOOL_H
