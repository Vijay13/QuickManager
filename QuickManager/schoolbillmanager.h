#ifndef SCHOOLBILLMANAGER_H
#define SCHOOLBILLMANAGER_H

#include <QTableView>
#include <QListView>
#include <QPushButton>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QProgressBar>
#include <allschool.h>

class SchoolBillManager
{
public:
    SchoolBillManager(QPushButton* editButton, QProgressBar* progressBar, QListView* list,
                      QTableView* headerTable,
                      QTableView* tableAttendence, QTableView* tableAttendenceTotal,
                      QTableView* tableBeneficiaries, QTableView* tableBeneficiariesTotal,
                      QLabel* taluka, QLabel* school);

    static SchoolBillManager* Instance();

    void navigatedToSBM(QString taluka, QString rout, int std, int period,
                        QString month, QString year);

    void setUpList();

    void setUpHeaderTable();

    void setUpTables();

    void resetTables();

    void setUpZeroinTable();

    void schoolChanged();

    bool checkData();

    void SchoolCenterNameChanged(QString centerNo);

    void SelectedCellChangedMainBillTable(QTableView* currentTable);

    void SelectedCellChangedHeaderTable();

    void EditSchoolEvent();

    void SaveSchoolEvent();

    void DeleteSchoolEvent();

    void setDates();

    void InsertDataToDatabase(QString dbTableName, QTableView* headerTable);

    void InsertDataToDatabase(QString dbTableName, QTableView* mainTable, QTableView* checkTable);

    void FillDataInTable(QString dbTableName, QStandardItemModel* headerTable);

    void FillDataInTable(QString dbTableName,
                         QStandardItemModel* viewMainTable, QStandardItemModel* viewCheckTable);

    QString getCurrentAttendenceTableName();

    QString getCurrentBeneficiariesTableName();

    int dataInTable(QTableView* tempTable, int r, int c);

    QString dataForTable(int data);

private:
    static SchoolBillManager* instance;
    QSqlQuery *query;
    QPushButton* editButton;
    MainDatabase *db;
    QProgressBar* progressBar;
    AllSchool* schools;

    QTableView* headerTable;
    QTableView* tableAttendence;
    QTableView* tableAttendenceTotal;
    QTableView* tableBeneficiaries;
    QTableView* tableBeneficiariesTotal;
    QListView* list;

    QStandardItemModel* viewModelList;
    QStandardItemModel* viewModelHeaderTable;
    QStandardItemModel* viewModelTableAttendence;
    QStandardItemModel* viewModelTableAttendenceTotal;
    QStandardItemModel* viewModelTableBeneficiaries;
    QStandardItemModel* viewModelTableBeneficiariesTotal;

    QSortFilterProxyModel* sortModelList;
    QSortFilterProxyModel* sortModelTableAttendence;
    QSortFilterProxyModel* sortModelTableAttendenceTotal;
    QSortFilterProxyModel* sortModelTableBeneficiaries;
    QSortFilterProxyModel* sortModelTableBeneficiariesTotal;

    QLabel* taluka;
    QLabel* schoolName;

    int previousRow,previousColumn;
    QTableView* currentTableTotal;
    QStandardItemModel* currentView;
    QString selectedTaluka, currentSchool, currentSTD, currentTaluka, currentCenter, currentRout,
    currentPeriod, currentMonth, currentYear;
    bool isTableReset;
};

#endif // SCHOOLBILLMANAGER_H
