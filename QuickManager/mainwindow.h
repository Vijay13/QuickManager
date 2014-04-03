#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QDebug>

#include "maindatabase.h"
#include "DataModel/alltaluka.h"
#include "DataModel/allschool.h"
#include "DataManager/filemanager.h"
#include "DataManager/schoolmanager.h"
#include "DataManager/schoolbillmanager.h"
#include "DataManager/foodlistmanager.h"
#include "talukamanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initialize();

    void initializeComponent();

    void updateBillOptions();

    void setUpControl(int TMAdd,int TMUpdate,int TMDelete,
                      int SMAdd,int SMUpdate,int SMDelete,
                      int SBMAdd, int SBMUpdate,int SBMDelete);

private slots:

    void LoginEvent();

    void LogoutEvent();

    void ChooseDatabaseEvent();

    void AddUserEvent();

    void AddAdminEvent();

    void TalukaManagerEvent();

    void SchoolManagerEvent();

    void SchoolBillManagerEvent();

    void SearchSchoolEvent();

    void setFilterEvent();

    void ResetSchoolEvent();

    void AddSchoolEvent();

    void EditSchoolEvent();

    void RemoveSchoolEvent();

    void PrintSchoolEvent();

    void ExportSchoolEvent();

    void BackEvent();

    void SelectedSchoolBillChanged(const QItemSelection& selection);

    void SelectedCellChangedAttendenceTable(const QItemSelection& selection);

    void SelectedCellChangedBeneficiariesTable(const QItemSelection& selection);

    void SelectedCellChangedHeaderTable(const QItemSelection& selection);

    void SchoolBillCenterFilterEvent();

    void EditSchoolBillEvent();

    void SaveSchoolBillEvent();

    void DeleteSchoolBillEvent();

    void setVisiblity(bool visible);

    void updateFLOptions();

    void FoodListEvent();

private:
    Ui::MainWindow *ui;
    QSqlQuery *query;
    MainDatabase *db;
    FileManager *fm;
    AllTaluka *talukas;
    AllSchool *schools;
    SchoolManager *sm;
    SchoolBillManager *sbm;
    FoodListManager *flm;
    bool isCorrect;
    QString queryStatement;
    QString dirToDatabase;
    int TMAdd, TMUpdate, TMDelete;

    QString getSpace(int count);
};

#endif // MAINWINDOW_H
