#include "alltaluka.h"

AllTaluka::AllTaluka()
{
    talukaList = new QList<Taluka*>();
    this->Initialize();

    instance = this;
}

void AllTaluka::Initialize(){
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());

    if(query->exec(db->getAllTalukaQuery())){
        while(query->next()){
            talukaList->append(new Taluka(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString()));
        }
    }else{
        qDebug() << "Didnt get all talukas";
    }
}

AllTaluka* AllTaluka::instance = 0;

AllTaluka* AllTaluka::Instance(){
    return instance;
}

void AllTaluka::addTaluka(Taluka* taluka){
    talukaList->append(taluka);
}

void AllTaluka::addTaluka(QString taluka, QString district){
    talukaList->append(new Taluka( taluka, district));
}

void AllTaluka::removeTaluka(QString taluka, QString district){
    if(query->exec(db->getDeleteTalukaQuery(taluka, district))){
        removeFromList(taluka, district);
    }else{
        qDebug() << "Could not delete Taluka";
    }
}

void AllTaluka::removeFromList(QString taluka, QString district){
   int i = -1;
   for(int j = 0; j<talukaList->length(); j++){
       if(talukaList->at(j)->getTaluka() == taluka && talukaList->at(j)->getDistrict() == district){
           i = j;
           break;
       }
   }
   if(i != -1){
       talukaList->removeAt(i);
   }
}

