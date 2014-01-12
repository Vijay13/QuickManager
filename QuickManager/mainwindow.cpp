
#include <QSqlRecord>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maindatabase.h"
#include "adduser.h"
#include "talukadialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), talukas(0), isCorrect(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeComponent();

    db = new MainDatabase();
    query = new QSqlQuery(*db->getDatabase());

    talukas = new AllTaluka();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeComponent(){
    ui->ErrorLable->setText("");
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->actionAdd_User->setDisabled(true);
    ui->actionAdd_Admin->setDisabled(true);
    ui->AllStackWidget->setCurrentIndex(0);
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
        if(isAdmin){
            ui->actionAdd_User->setEnabled(true);
            ui->actionAdd_Admin->setEnabled(true);
        }
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

void MainWindow::on_actionAdd_User_triggered()
{
    AddUser *addUser = new AddUser( this, true);
    addUser->setWindowTitle("Add User");
    addUser->open();
}

void MainWindow::on_actionAdd_Admin_triggered()
{
    AddUser *addUser = new AddUser( this, false);
    addUser->setWindowTitle("Add Admin");
    addUser->open();
}

void MainWindow::on_pushButtonAddTaluka_clicked()
{
    TalukaDialog *t = new TalukaDialog();
    t->open();
}
