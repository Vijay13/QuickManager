#ifndef FOODLISTMANAGER_H
#define FOODLISTMANAGER_H

#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QProgressBar>
#include <QStyledItemDelegate>

#include "DataModel/allschool.h"

/*
 * Manages operation of foodlist page of mainwindow
 * */
class FoodListManager
{
public:
    FoodListManager(QTableView *tableFoodList, QProgressBar *progressBar);

    void setUpTables(QString date,QString rout);

    void fillDataInTable();

    void deleteFoodListEvent();

public slots:
    void saveFoodListEvent();

private:
    QSqlQuery *query;
    MainDatabase *db;
    QProgressBar *progressBar;
    AllSchool *schools;
    QString date,rout;

    QTableView* table;
    QStandardItemModel* viewModel;
    QSortFilterProxyModel* sortModel;

    void setUpHeaderTable();

    QString getTableName();

    void insertDataToTable(QString tableName, QTableView* table);

    int dataInTable(QTableView *tempTable, int r, int c) const;

};

#endif // FOODLISTMANAGER_H
