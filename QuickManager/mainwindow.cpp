
#include <QSqlRecord>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maindatabase.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), isCorrect(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ErrorLable->setText("");

    db = new MainDatabase();
    query = new QSqlQuery(*db->getDatabase());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonLogin_clicked()
{
    int isAdmin = ui->comboBoxUserAdmin->currentIndex();
    isCorrect = false;
    queryStatement = "SELECT * FROM USERS WHERE isAdmin = " + QString::number(isAdmin);

    if(query->exec(queryStatement)){
        int userFieldAt = query->record().indexOf("UserName");
        int passwordFieldAt = query->record().indexOf("Password");
        while(query->next()){
            QString userName = query->value(userFieldAt).toString();
            QString passWord = query->value(passwordFieldAt).toString();
            if(userName == ui->lineEditUsername->text() && passWord == ui->lineEditUsername->text()){
                isCorrect = true;
            }
        }
    }

    if(isCorrect){
        ui->AllStackWidget->setCurrentIndex(1);
    }else{
        ui->ErrorLable->setText("Incorrect Username or Password");
    }
}

void MainWindow::on_actionLog_out_triggered()
{
    ui->AllStackWidget->setCurrentIndex(0);
    ui->lineEditUsername->setText("");
    ui->lineEditPassword->setText("");
    ui->ErrorLable->setText("");
}
