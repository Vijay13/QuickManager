#include "taluka.h"

Taluka::Taluka(): TID(-1), TalukaName(""), DistrictName(""), inserted(false){
    this->Initialize();
}

Taluka::Taluka(int tid, QString talukaName, QString districtName): TID(tid), TalukaName(talukaName), DistrictName(districtName), inserted(true){
    this->Initialize();
}

Taluka::Taluka(QString talukaName, QString districtName) : TID(-1), TalukaName(talukaName), DistrictName(districtName), inserted(false)
{
    this->Initialize();
    if(query->exec(db->getInsertTalukaQuery( TalukaName, DistrictName))){
        inserted = true;
    }else{
        qDebug() << "Taluka not inserted";
    }
}

void Taluka::Initialize(){
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());
}

void Taluka::updateTaluka(int TID, QString newTalukaValue, QString newDistrictValue){
    if(query->exec(db->getUpdateTalukaQuery( TID, newTalukaValue, newDistrictValue))){
        inserted = true;
    }else{
        qDebug() << "Taluka not updated";
    }
}

int Taluka::getTID(){
    if(query->exec("SELECT * FROM TALUKAS WHERE Taluka = '" + TalukaName + "' AND District = '" + DistrictName + "' );")){
        while(query->next()){
            return query->value(0).toInt();
        }
    }
    qDebug() << "Could not find TID";
    return -1;
}
