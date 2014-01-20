#ifndef SCHOOLBILLMANAGER_H
#define SCHOOLBILLMANAGER_H

#include <QTableView>
#include <QListView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QProgressBar>
#include <allschool.h>

class SchoolBillManager
{
public:
    SchoolBillManager(QProgressBar* progressBar, QListView* list,QTableView* headerTable, QTableView* table,
                      QLabel* center, QLabel* school);

    static SchoolBillManager* Instance();

    void navigatedToSBM(QString taluka, QString rout, int std, int period,
                     QString month, QString year);

    void setUpHeaderTable();

    void setUpTable();

    void resetTable();

    void setUpZeroinTable();

    void schoolChanged();

    bool checkData();

    int dataInTable(QTableView* tempTable, int r, int c);

    QString dataForTable(int data);

    void SelectedCellChangedBillTable();

    void SelectedCellChangedHeaderTable();

    void SaveSchoolEvent();

    void DeleteSchoolEvent();

    void setDates();

    QString getCurrentTableName();

private:
    static SchoolBillManager* instance;
    QSqlQuery *query;
    MainDatabase *db;
    QProgressBar* progressBar;
    QTableView* headerTable;
    QTableView* table;
    QListView* list;
    AllSchool* schools;
    QStandardItemModel* viewModelTable;
    QStandardItemModel* viewModelHeaderTable;
    QSortFilterProxyModel* sortModelTable;
    QStandardItemModel* viewModelList;
    QSortFilterProxyModel* sortModelList;
    QLabel* centerNo;
    QLabel* schoolName;
    int previousRow,previousColumn;
    QString currentSchool, currentSTD, currentPeriod, currentMonth, currentYear;
    bool isTableReset;
};

#endif // SCHOOLBILLMANAGER_H
