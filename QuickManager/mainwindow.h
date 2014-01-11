#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QDebug>

#include "maindatabase.h"

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
    void on_pushButtonLogin_clicked();

    void on_actionLog_out_triggered();

private:
    Ui::MainWindow *ui;
    QSqlQuery *query;
    MainDatabase *db;
    QString queryStatement;
    bool isCorrect;
};

#endif // MAINWINDOW_H
