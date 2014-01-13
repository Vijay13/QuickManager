#ifndef SCHOOLMANAGER_H
#define SCHOOLMANAGER_H

#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <allschool.h>

class SchoolManager
{
public:
    SchoolManager(QTableView* table);

    static SchoolManager* Instance();

    void setUpTable();

    void addSchool();

    void editSchool();

    void removeSchool();

private:
    static SchoolManager* instance;
    QTableView* table;
    AllSchool* schools;
    QStandardItemModel* viewModel;
    QSortFilterProxyModel* sortModel;
};

#endif // SCHOOLMANAGER_H