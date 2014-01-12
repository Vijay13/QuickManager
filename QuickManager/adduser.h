#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDebug>
#include <QDialog>
#include <QSqlQuery>
#include <maindatabase.h>

namespace Ui {
class AddUser;
}

class AddUser : public QDialog
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = 0, bool isuser = true);
    ~AddUser();

    bool Check();

    void clearFields();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonReset_clicked();

    void on_pushButtonCancle_clicked();

private:
    Ui::AddUser *ui;
    MainDatabase *db;
    QSqlQuery *query;
    bool isUser;
};

#endif // ADDUSER_H
