#ifndef ALLSCHOOL_H
#define ALLSCHOOL_H

#include <DataModel/school.h>

class AllSchool
{
public:
    AllSchool();

    static AllSchool* Instance();

    void Initialize();

    void addSchool(School* school);

    void removeSchoolFromDB(QString centerNo, QString taluka);

    void removeFromList(QString centerNo, QString taluka);

    int indexOfSchoolWith(QString centerNo, QString taluka);

    int indexOfSchoolWith(int sid);

    School* getSchool(QString schoolName);

    QList<School*>* getSchoolList(){return schoolList;}

    void setAllRouts();

    QList<QString>* getAllRouts(){return routList;}

private:
    static AllSchool* instance;
    QList<School*>* schoolList;
    QList<QString>* routList;
    MainDatabase *db;
    QSqlQuery *query;
};

#endif // ALLSCHOOL_H
