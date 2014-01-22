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
                      QLabel* taluka, QLabel* school);

    static SchoolBillManager* Instance();

    void navigatedToSBM(QString taluka, QString rout, int std, int period,
                        QString month, QString year);

    void setUpList();

    void setUpHeaderTable();

    void setUpTable();

    void resetTables();

    void setUpZeroinTable();

    void schoolChanged();

    bool checkData();

    void SchoolCenterNameChanged(QString centerNo);

    void SelectedCellChangedBillTable();

    void SelectedCellChangedHeaderTable();

    void SaveSchoolEvent();

    void DeleteSchoolEvent();

    void setDates();

    QString getCurrentTableName();

    int dataInTable(QTableView* tempTable, int r, int c);

    QString dataForTable(int data);

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
    QLabel* taluka;
    QLabel* schoolName;
    int previousRow,previousColumn;
    QString currentSchool, currentSTD, currentTaluka, currentRout,
    currentPeriod, currentMonth, currentYear;
    bool isTableReset;
};

#endif // SCHOOLBILLMANAGER_H
