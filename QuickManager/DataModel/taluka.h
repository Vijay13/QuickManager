#ifndef TALUKA_H
#define TALUKA_H

#include <QDebug>
#include <QString>
#include "maindatabase.h"

/*
 * This class contains details of perticular taluka
 * */

class Taluka
{
public:

    /*
     * creates empty taluka object whose parameters can be set later.
     * */
    Taluka();

    /*
     * creates taluka object and enter data into database
     * */
    Taluka(QString talukaName, QString districtName);

    /*
     * creates taluka object whose data is already inserted into database.
     * */
    Taluka(int tid, QString talukaName, QString districtName);

    void Initialize();

    void setTaluka(QString taluka){ TalukaName = taluka;}

    void setDistrict(QString district){DistrictName = district;}

    int getTID(){return TID;}

    void setTID();

    QString getTaluka(){return TalukaName;}

    QString getDistrict(){return DistrictName;}

private:
    int TID;
    QString TalukaName;
    QString DistrictName;
    bool inserted;
    MainDatabase *db;
    QSqlQuery *query;
};

#endif // TALUKA_H
