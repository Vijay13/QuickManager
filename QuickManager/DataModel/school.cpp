#include "school.h"

School::School()
{
    this->Initialize();
}

School::School(int sid,QString centerNo,QString taluka,QString schoolName,QString address,QString principal,
               QString principalMobNo,QString principalLandline,
               QString teacher,QString teacherMobno,QString principalAddress,
               QString routNo,QString girlMixboy,QString type,QString isprimary) : SID(sid),
    CenterNo(centerNo), Taluka(taluka), SchoolName(schoolName), Address(address),
    Principal(principal), PrincipalMobNo(principalMobNo), PrincipalLandline(principalLandline),
    Teacher(teacher), TeacherMobNo(teacherMobno), PrincipalAddress(principalAddress),RoutNo(routNo),
    girlMIXboy(girlMixboy), Type(type), isPrimary(isprimary)
{
}

School::School(QString centerNo,QString taluka,QString schoolName,QString address,QString principal,
               QString principalMobNo,QString principalLandline,
               QString teacher,QString teacherMobno,QString principalAddress,
               QString routNo,QString girlMixboy,QString type,QString isprimary) :
    CenterNo(centerNo), Taluka(taluka), SchoolName(schoolName), Address(address),
    Principal(principal), PrincipalMobNo(principalMobNo), PrincipalLandline(principalLandline),
    Teacher(teacher), TeacherMobNo(teacherMobno), PrincipalAddress(principalAddress),RoutNo(routNo),
    girlMIXboy(girlMixboy), Type(type), isPrimary(isprimary)
{
    this->Initialize();
    if(query->exec(db->getInsertSchoolQuery(CenterNo,Taluka,
                                            SchoolName,Address,Principal,
                                            PrincipalMobNo,PrincipalLandline,Teacher,
                                            TeacherMobNo,PrincipalAddress,RoutNo,
                                            girlMIXboy,Type,isPrimary))){
    }else{
        qDebug() << "School could not be inserted";
    }
}

void School::Initialize(){
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());
}
