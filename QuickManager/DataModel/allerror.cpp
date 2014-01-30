#include "allerror.h"

AllError::AllError()
{
    this->errorList = new QList<QMError*>();
    this->Initialize();

    instance = this;
}

AllError* AllError::instance = 0;

AllError* AllError::Instance(){
    return instance;
}

void AllError::Initialize()
{
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());

    if(query->exec(db->getAllErrorsQuery())){
        while(query->next()){
            errorList->append(new QMError(query->value(0).toInt(), query->value(1).toString(),
                                          query->value(2).toString(),query->value(3).toString(),
                                          query->value(4).toString(),query->value(5).toString()));
        }
    }else{
        qDebug() << "Didnt get all errors";
    }
    query->finish();
}

void AllError::addError(QString user, QString disc, QString status, QString date, QString model)
{
    errorList->append(new QMError(user,disc,status,date,model));
}

int AllError::getErrorPos(QString disc, QString date)
{
    int i = -1;
    for(int j = 0; j<errorList->length(); j++){
        if(errorList->at(j)->disc == disc && errorList->at(j)->date == date){
            i = j;
            break;
        }
    }
    return i;
}

QMError* AllError::getError(QString disc, QString date)
{
    int pos = this->getErrorPos(disc,date);
    return (pos == -1) ? 0 : errorList->at(pos);
}

void AllError::removeError(QString disc, QString date)
{
    int pos = this->getErrorPos(disc,date);
    if(pos != -1 && pos < errorList->length() && query->exec(db->getDeleteErrorQuery(disc,date)))
    {
        errorList->removeAt(pos);
    }else
        qDebug() << "Couldnot delete error";
}
