#ifndef TALUKADIALOG_H
#define TALUKADIALOG_H

#include <QDebug>
#include <QDialog>
#include <alltaluka.h>
#include <maindatabase.h>

namespace Ui {
class TalukaDialog;
}

class TalukaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TalukaDialog(QWidget *parent = 0, int tid = -1);
    ~TalukaDialog();

    bool Check();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonCancle_clicked();

private:
    Ui::TalukaDialog *ui;
    AllTaluka* talukas;
    MainDatabase *db;
    QSqlQuery *query;
    int TID;
    QString oldTaluka,oldDistrict;
};

#endif // TALUKADIALOG_H
