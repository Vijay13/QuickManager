#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QDebug>

#include "maindatabase.h"
#include "alltaluka.h"
#include "allschool.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool isCorrect;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void initializeComponent();

    void on_pushButtonLogin_clicked();

    void on_actionLog_out_triggered();

    void on_actionAdd_User_triggered();

    void on_actionAdd_Admin_triggered();

    void on_pushButtonAddTaluka_clicked();

    void on_pushButtonAddTaluka_2_clicked();

private:
    Ui::MainWindow *ui;
    QSqlQuery *query;
    MainDatabase *db;
    AllTaluka *talukas;
    AllSchool *schools;
    QString queryStatement;
};

#endif // MAINWINDOW_H
