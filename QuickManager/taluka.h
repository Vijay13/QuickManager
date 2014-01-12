#ifndef TALUKA_H
#define TALUKA_H

#include <QDebug>
#include <QString>
#include "maindatabase.h"

class Taluka
{
public:
    bool inserted;

    Taluka();

    Taluka(QString talukaName, QString districtName);

    Taluka(int tid, QString talukaName, QString districtName);

    void Initialize();

    void setTaluka(QString taluka){ TalukaName = taluka;}

    void setDistrict(QString district){DistrictName = district;}

    void updateTaluka(int TID, QString newTalukaValue, QString newDistrictValue);

    int getTID();

    QString getTaluka(){return TalukaName;}

    QString getDistrict(){return DistrictName;}

private:
    int TID;
    QString TalukaName;
    QString DistrictName;
    MainDatabase *db;
    QSqlQuery *query;
};

#endif // TALUKA_H
