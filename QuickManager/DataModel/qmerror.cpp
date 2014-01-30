#include "qmerror.h"

QMError::QMError()
{
    EID = -1;
    user = "";
    disc = "";
    status = "";
    date = "";
    model = "";
    this->Initialize();
}

QMError::QMError(QString user,QString disc,QString status,QString date,QString model):
    EID(-1),
    user(user),disc(disc),status(status),date(date),model(model)
{
    this->Initialize();
    if(query->exec(db->getInsertErrorQuery(user,disc,status,date,model)))
    {
        this->setEID();
    }else
        qDebug() << "Could not insert error";
}

QMError::QMError(int eid,QString user,QString disc,QString status,QString date,QString model):
    EID(eid),
    user(user),disc(disc),status(status),date(date),model(model)
{

}

void QMError::Initialize()
{
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());
}

void QMError::setEID(){
    if(query->exec("SELECT * FROM ERRORS WHERE DISC = '" + disc + "' AND DATE = '" + date + "';")){
        while(query->next()){
            EID = query->value(0).toInt();
            return;
        }
    }
    qDebug() << "Could not find TID";
    EID = -1;
}
