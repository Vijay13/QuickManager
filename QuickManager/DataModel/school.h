#ifndef SCHOOL_H
#define SCHOOL_H

#include <QString>
#include <QDebug>
#include <maindatabase.h>

/*
 * This class contains details of perticular School
 * */

class School
{
public:

    /*
     * constructor
     * creates school object whose parameters can be set later
     * */
    School();

    /*
     * constructor
     * sets parameter of school object
     * */
    School(int sid,QString centerNo,QString taluka,QString schoolName,QString address,QString principal,
           QString principalMobNo,QString principalLandline,
           QString teacher,QString teacherMobno,QString principalAddress,
           QString routNo,QString girlMixboy,QString type,QString isprimary);

    /*
     * constructor
     * sets parameter of school object and enters data into database
     * */
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
