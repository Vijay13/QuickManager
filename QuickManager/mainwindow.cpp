
#include <QSqlRecord>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maindatabase.h"
#include "adduser.h"
#include "talukadialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isCorrect(false), queryStatement(""), dirToDatabase("")
{
    ui->setupUi(this);
    initializeComponent();

    db = new MainDatabase();
    db->Open(dirToDatabase);
    query = new QSqlQuery(*db->getDatabase());

    talukas = new AllTaluka();
    schools = new AllSchool();
    sm = new SchoolManager(ui->tableViewSchools);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeComponent(){
    this->setWindowTitle("Quick Manager Akshaypatra");
    ui->ErrorLable->setText("");
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->actionAddUser->setDisabled(true);
    ui->actionAddAdmin->setDisabled(true);
    ui->AllStackWidget->setCurrentIndex(1);

    QObject::connect(ui->actionChooseDatabase,SIGNAL(triggered()),this,SLOT(ChooseDatabase()));
    QObject::connect(ui->pushButtonLogin,SIGNAL(clicked()),this,SLOT(Login()));
    QObject::connect(ui->actionLogout,SIGNAL(triggered()),this,SLOT(Logout()));
    QObject::connect(ui->actionAddUser,SIGNAL(triggered()),this,SLOT(AddUserEvent()));
    QObject::connect(ui->actionAddAdmin,SIGNAL(triggered()),this,SLOT(AddAdminEvent()));
    QObject::connect(ui->pushButtonSchoolManager,SIGNAL(clicked()),this,SLOT(SchoolManagerEvent()));
    QObject::connect(ui->pushButtonAddTaluka,SIGNAL(clicked()),this,SLOT(AddTalukaEvent()));
    QObject::connect(ui->pushButtonAddSchool,SIGNAL(clicked()),this,SLOT(AddSchoolEvent()));
    QObject::connect(ui->pushButtonEditSchool,SIGNAL(clicked()),this,SLOT(EditSchoolEvent()));
    QObject::connect(ui->pushButtonRemoveSchool,SIGNAL(clicked()),this,SLOT(RemoveSchoolEvent()));
}

void MainWindow::ChooseDatabase()
{
    dirToDatabase =  QFileDialog::getExistingDirectory(this, "Select Folder","/",QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);

    db = new MainDatabase();
    db->Open(dirToDatabase);
    query = new QSqlQuery(*db->getDatabase());
}

void MainWindow::Login()
{
    int isAdmin = ui->comboBoxUserAdmin->currentIndex();
    isCorrect = false;
    queryStatement = "SELECT * FROM USERS WHERE isAdmin = " + QString::number(isAdmin);

    if(query->exec(queryStatement)){
        while(query->next()){
            QString userName = query->value(query->record().indexOf("UserName")).toString();
            QString passWord = query->value(query->record().indexOf("Password")).toString();
            if(userName == ui->lineEditUsername->text() && passWord == ui->lineEditUsername->text()){
                isCorrect = true;
                break;
            }
        }
    }

    if(isCorrect){
        ui->AllStackWidget->setCurrentIndex(1);
        if(isAdmin){
            ui->actionAddUser->setEnabled(true);
            ui->actionAddAdmin->setEnabled(true);
        }
    }else{
        ui->ErrorLable->setText("Incorrect Username or Password");
    }
}

void MainWindow::Logout()
{
    ui->AllStackWidget->setCurrentIndex(0);
    ui->lineEditUsername->setText("");
    ui->lineEditPassword->setText("");
    ui->ErrorLable->setText("");
}

void MainWindow::AddUserEvent()
{
    AddUser *addUser = new AddUser( this, true);
    addUser->setWindowTitle("Add User");
    addUser->open();
}

void MainWindow::AddAdminEvent()
{
    AddUser *addUser = new AddUser( this, false);
    addUser->setWindowTitle("Add Admin");
    addUser->open();
}

void MainWindow::AddTalukaEvent()
{
    TalukaDialog *t = new TalukaDialog();
    t->setWindowTitle("Add Taluka Information");
    t->open();
}

void MainWindow::SchoolManagerEvent()
{
    ui->AllStackWidget->setCurrentIndex(2);
}

void MainWindow::AddSchoolEvent(){
    sm->addSchool();
}

void MainWindow::EditSchoolEvent(){
    sm->editSchool();
}

void MainWindow::RemoveSchoolEvent(){
    sm->removeSchool();
}
