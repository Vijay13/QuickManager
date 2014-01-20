#include "allschool.h"

AllSchool::AllSchool()
{
    schoolList = new QList<School*>();
    routList = new QList<QString>();
    this->Initialize();

    instance = this;
}

AllSchool* AllSchool::instance = 0;

AllSchool* AllSchool::Instance(){
    return instance;
}

void AllSchool::Initialize(){
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());

    if(query->exec(db->getAllSchoolQuery())){
        while(query->next()){
            schoolList->append(new School(query->value(0).toInt(), query->value(1).toString(),
                                          query->value(2).toString(),
                                          query->value(3).toString(),
                                          query->value(4).toString(),
                                          query->value(5).toString(),
                                          query->value(6).toString(),
                                          query->value(7).toString(),
                                          query->value(8).toString(),
                                          query->value(9).toString(),
                                          query->value(10).toString(),
                                          query->value(11).toString(),
                                          query->value(12).toString(),
                                          query->value(13).toString(),
                                          query->value(14).toString() ));
        }
    }else{
        qDebug() << "Didnt get all talukas";
    }

    setAllRouts();
}

School* AllSchool::getSchool(QString schoolName)
{
    for(int i=0; i < schoolList->length(); i++)
    {
        if(schoolList->at(i)->SchoolName == schoolName)
            return schoolList->at(i);
    }

    return 0;
}

void AllSchool::addSchool(School* school){
    schoolList->append(school);
    setAllRouts();
}

void AllSchool::removeSchoolFromDB(QString centerNo, QString taluka){
    if(query->exec(db->getDeleteSchoolQuery(centerNo, taluka))){
        removeFromList(centerNo, taluka);
        setAllRouts();
    }else{
        qDebug() << "Could not remove school";
    }
}

void AllSchool::removeFromList(QString centerNo, QString taluka){
   int i = indexOfSchoolWith(centerNo,taluka);
   if(i > -1){
       schoolList->removeAt(i);
   }
}

int AllSchool::indexOfSchoolWith(QString centerNo, QString taluka){
    int i = -1;
    for(int j = 0; j<schoolList->length(); j++){
        if(schoolList->at(j)->CenterNo == centerNo && schoolList->at(j)->Taluka == taluka){
            i = j;
            break;
        }
    }
    return i;
}

int AllSchool::indexOfSchoolWith(int sid){
    int i = -1;
    for(int j = 0; j<schoolList->length(); j++){
        if(schoolList->at(j)->SID == sid){
            i = j;
            break;
        }
    }
    return i;
}

void AllSchool::setAllRouts(){
    routList->clear();
    if(query->exec("SELECT DISTINCT RoutNo FROM SCHOOLS"))
    {
        while (query->next()) {
            routList->append(query->value(0).toString());
        }
    }else{
        qDebug() << "Could not get Rout nos";
    }
}
