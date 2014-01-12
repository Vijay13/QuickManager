#include "allschool.h"

AllSchool::AllSchool()
{
    schoolList = new QList<School*>();
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
}

void AllSchool::addSchool(School* school){
    schoolList->append(school);
}

void AllSchool::removeSchool(QString centerNo, QString taluka){
    if(query->exec(db->getDeleteSchoolQuery(centerNo, taluka))){
        removeFromList(centerNo, taluka);
    }else{
        qDebug() << "Could not remove school";
    }
}

void AllSchool::removeFromList(QString centerNo, QString taluka){
   int i = -1;
   for(int j = 0; j<schoolList->length(); j++){
       if(schoolList->at(j)->CenterNo == centerNo && schoolList->at(j)->Taluka == taluka){
           i = j;
           break;
       }
   }
   if(i != -1){
       schoolList->removeAt(i);
   }
}
