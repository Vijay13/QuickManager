#ifndef SCHOOL_H
#define SCHOOL_H

#include <QString>
#include <QDebug>
#include <maindatabase.h>

class School
{
public:
    School();

    School(int sid,QString centerNo,QString taluka,QString schoolName,QString address,QString principal,
           QString principalMobNo,QString principalLandline,
           QString teacher,QString teacherMobno,QString principalAddress,
           QString routNo,QString girlMixboy,QString type,QString isprimary);

    School(QString centerNo,QString taluka,QString schoolName,QString address,QString principal,
                   QString principalMobNo,QString principalLandline,
                   QString teacher,QString teacherMobno,QString principalAddress,
                   QString routNo,QString girlMixboy,QString type,QString isprimary);

    void Initialize();

    int SID;

    QString CenterNo, Taluka, SchoolName, Address, Principal, PrincipalMobNo, PrincipalLandline,
        Teacher, TeacherMobNo, PrincipalAddress, RoutNo, girlMIXboy, Type, isPrimary;

private:
    MainDatabase *db;
    QSqlQuery *query;
};

#endif // SCHOOL_H
