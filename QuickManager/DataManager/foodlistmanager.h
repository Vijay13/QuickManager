#ifndef FOODLISTMANAGER_H
#define FOODLISTMANAGER_H

#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QProgressBar>
#include <QStyledItemDelegate>

#include "DataModel/allschool.h"

class FoodListManager
{
public:
    FoodListManager(QTableView *tableFoodList);

    void setUpTables(QString rout);
private:
    QSqlQuery *query;
    MainDatabase *db;
    QProgressBar *progressBar;
    AllSchool *schools;

    QTableView* table;
    QStandardItemModel* viewModel;
    QSortFilterProxyModel* sortModel;

    void setUpHeaderTable();

};

#endif // FOODLISTMANAGER_H
