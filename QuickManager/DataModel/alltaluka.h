#ifndef ALLTALUKA_H
#define ALLTALUKA_H

#include <QList>
#include <maindatabase.h>
#include <DataModel/taluka.h>

/*
 * This class contains list of all taluka
 * Any taluka is added or removed from here only
 * */
class AllTaluka
{
public:

    /*
     * Constructor
     * Initialise the list of taluka i.e. talukaList
     * */
    AllTaluka();

    static AllTaluka* Instance();

    void Initialize();

    /*
     * Add Taluka to talukaList
     * */
    void addTaluka(Taluka* taluka);

    /*
     * creates and add taluka to talukaList
     * */
    void addTaluka(QString taluka, QString district);

    /*
     * return perticular taluka from tid
     * */
    Taluka* getTaluka(int TID);

    void removeTaluka(QString taluka, QString district);

    void removeFromList(QString taluka, QString district);

    QList<Taluka*>* getTalukaList(){return talukaList;}

private:
    static AllTaluka* instance;
    QList<Taluka*>* talukaList;
    MainDatabase *db;
    QSqlQuery *query;
};

#endif // ALLTALUKA_H
