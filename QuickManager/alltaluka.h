#ifndef ALLTALUKA_H
#define ALLTALUKA_H

#include <QList>
#include <maindatabase.h>
#include <taluka.h>

class AllTaluka
{
public:
    AllTaluka();

    static AllTaluka* Instance();

    void Initialize();

    void addTaluka(Taluka* taluka);

    void addTaluka(QString taluka, QString district);

    void removeTaluka(QString taluka, QString district);

    void removeFromList(QString taluka, QString district);

    QString getAllTalukaQuery();

    QList<Taluka*>* getTalukaList(){return talukaList;}

private:
    static AllTaluka* instance;
    QList<Taluka*>* talukaList;
    MainDatabase *db;
    QSqlQuery *query;
};

#endif // ALLTALUKA_H
