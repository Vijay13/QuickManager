#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QDebug>

#include "maindatabase.h"
#include "alltaluka.h"
#include "allschool.h"
#include "filemanager.h"
#include "talukamanager.h"
#include "schoolmanager.h"
#include "schoolbillmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void initializeComponent();

    void updateBillOptions();

    void Login();

    void Logout();

    void ChooseDatabase();

    void AddUserEvent();

    void AddAdminEvent();

    void TalukaManagerEvent();

    void SchoolManagerEvent();

    void SchoolBillManagerEvent();

    void SearchSchool();

    void setFilter();

    void ResetSchool();

    void AddSchoolEvent();

    void EditSchoolEvent();

    void RemoveSchoolEvent();

    void PrintSchoolEvent();

    void ExportSchoolEvent();

    void BackEvent();

    void SelectedSchoolBillChanged(const QItemSelection& selection);

    void SelectedCellChangedBillTable(const QItemSelection& selection);

    void SelectedCellChangedHeaderTable(const QItemSelection& selection);

    void SaveSchoolBillEvent();

    void DeleteSchoolBillEvent();

private:
    Ui::MainWindow *ui;
    QSqlQuery *query;
    MainDatabase *db;
    FileManager *fm;
    AllTaluka *talukas;
    AllSchool *schools;
    SchoolManager *sm;
    SchoolBillManager *sbm;
    bool isCorrect;
    QString queryStatement;
    QString dirToDatabase;
};

#endif // MAINWINDOW_H
