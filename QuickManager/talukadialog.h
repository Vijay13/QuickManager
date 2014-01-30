#ifndef TALUKADIALOG_H
#define TALUKADIALOG_H

#include <QDebug>
#include <QDialog>
#include "DataModel/alltaluka.h"
#include "maindatabase.h"

namespace Ui {
class TalukaDialog;
}

class TalukaDialog : public QDialog
{
    Q_OBJECT

public:
    int TID;
    QString oldTaluka,oldDistrict;

    explicit TalukaDialog(QWidget *parent = 0, int tid = -1);
    ~TalukaDialog();

    bool Check();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonCancle_clicked();

signals:
    void closed();

private:
    Ui::TalukaDialog *ui;
    AllTaluka* talukas;
    MainDatabase *db;
    QSqlQuery *query;

    void setUpFields();
};

#endif // TALUKADIALOG_H
