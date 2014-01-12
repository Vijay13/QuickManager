#ifndef SCHOOLDIALOG_H
#define SCHOOLDIALOG_H

#include <QDialog>
#include <alltaluka.h>
#include <allschool.h>
#include <maindatabase.h>

namespace Ui {
class SchoolDialog;
}

class SchoolDialog : public QDialog
{
    Q_OBJECT

public:
    int SID;
    explicit SchoolDialog(QWidget *parent = 0, int sid = -1);
    ~SchoolDialog();

    void initializeComponent();

    void Initialize();

    bool Check();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonCancle_clicked();

private:
    Ui::SchoolDialog *ui;
    AllTaluka* talukas;
    AllSchool* schools;
    MainDatabase *db;
    QSqlQuery *query;
    QString CenterNo, Taluka, SchoolName, Address, Principal, PrincipalMobNo, PrincipalLandline,
        Teacher, TeacherMobNo, PrincipalAddress, RoutNo, girlMIXboy, Type, isPrimary;
};

#endif // SCHOOLDIALOG_H
