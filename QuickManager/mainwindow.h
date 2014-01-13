#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QDebug>

#include "maindatabase.h"
#include "alltaluka.h"
#include "allschool.h"
#include "talukamanager.h"
#include "schoolmanager.h"

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

    void Login();

    void Logout();

    void ChooseDatabase();

    void AddUserEvent();

    void AddAdminEvent();

    void TalukaManagerEvent();

    void SchoolManagerEvent();

    void AddSchoolEvent();

    void EditSchoolEvent();

    void RemoveSchoolEvent();

    void BackEvent();

private:
    Ui::MainWindow *ui;
    QSqlQuery *query;
    MainDatabase *db;
    AllTaluka *talukas;
    AllSchool *schools;
    SchoolManager *sm;
    bool isCorrect;
    QString queryStatement;
    QString dirToDatabase;
};

#endif // MAINWINDOW_H
