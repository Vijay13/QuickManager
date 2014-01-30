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
        this->setTID();
        inserted = true;
    }else{
        qDebug() << "Taluka not inserted";
    }
}

void Taluka::Initialize(){
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());
}

void Taluka::setTID(){
    if(query->exec("SELECT * FROM TALUKAS WHERE Taluka = '" + TalukaName + "' AND District = '" + DistrictName + "';")){
        while(query->next()){
            TID = query->value(0).toInt();
            return;
        }
    }
    qDebug() << "Could not find TID";
    TID = -1;
}
