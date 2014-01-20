
#include <QSqlRecord>
#include <QFileDialog>
#include <QDateTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maindatabase.h"
#include "adduser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isCorrect(false), queryStatement(""), dirToDatabase("")
{
    ui->setupUi(this);

    db = new MainDatabase();
    db->Open(dirToDatabase);
    query = new QSqlQuery(*db->getDatabase());

    fm = new FileManager();
    talukas = new AllTaluka();
    schools = new AllSchool();
    sm = new SchoolManager(ui->tableViewSchools);
    sbm = new SchoolBillManager(ui->progressBar,ui->listViewSchools ,ui->tableViewTotalBill ,ui->tableViewMainBill,
                                ui->labelBMCenterBody, ui->labelBMSchoolBody);
    initializeComponent();
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
    ui->AllStackWidget->setCurrentIndex(0);
    ui->groupBoxControl->setVisible(false);
    ui->progressBar->setVisible(false);

    this->updateBillOptions();

    QObject::connect(ui->actionChooseDatabase,SIGNAL(triggered()),this,SLOT( ChooseDatabase()));
    QObject::connect(ui->pushButtonLogin,SIGNAL(clicked()),this,SLOT( Login()));
    QObject::connect(ui->actionLogout,SIGNAL(triggered()),this,SLOT( Logout()));
    QObject::connect(ui->actionAddUser,SIGNAL(triggered()),this,SLOT( AddUserEvent()));
    QObject::connect(ui->actionAddAdmin,SIGNAL(triggered()),this,SLOT( AddAdminEvent()));

    QObject::connect(ui->pushButtonSchoolManager,SIGNAL(clicked()),this,SLOT( SchoolManagerEvent()));
    QObject::connect(ui->pushButtonTalukaManager,SIGNAL(clicked()),this,SLOT( TalukaManagerEvent()));

    QObject::connect(ui->pushButtonAddSchool,SIGNAL(clicked()),this,SLOT( AddSchoolEvent()));
    QObject::connect(ui->pushButtonEditSchool,SIGNAL(clicked()),this,SLOT( EditSchoolEvent()));
    QObject::connect(ui->pushButtonRemoveSchool,SIGNAL(clicked()),this,SLOT( RemoveSchoolEvent()));
    QObject::connect(ui->pushButtonBack,SIGNAL(clicked()),this,SLOT( BackEvent()));

    QObject::connect(ui->pushButtonPrintSchools,SIGNAL(clicked()),this,SLOT( PrintSchoolEvent()));
    QObject::connect(ui->pushButtonExportSchools,SIGNAL(clicked()),this,SLOT( ExportSchoolEvent()));

    QObject::connect(ui->lineEditSearchBox,SIGNAL(textChanged(const QString &)), SLOT( SearchSchool()) );
    QObject::connect(ui->comboBoxSelectTaluka,SIGNAL(activated(int)), SLOT( setFilter()) );
    QObject::connect(ui->comboBoxSelectRout,SIGNAL(activated(int)), SLOT( setFilter()) );
    QObject::connect(ui->pushButtonResetSchoolSearch,SIGNAL(clicked()), SLOT( ResetSchool()) );

    QObject::connect(ui->pushButtonBMManager,SIGNAL(clicked()), SLOT( SchoolBillManagerEvent()) );
    QObject::connect(ui->pushButtonSaveSchoolBill,SIGNAL(clicked()),SLOT(SaveSchoolBillEvent()));
    QObject::connect(ui->pushButtonDeleteSchoolBill,SIGNAL(clicked()),SLOT(DeleteSchoolBillEvent()));
    QObject::connect(ui->listViewSchools->selectionModel(),
                     SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(SelectedSchoolBillChanged(QItemSelection)));
    QObject::connect(ui->tableViewMainBill->selectionModel(),
                     SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(SelectedCellChangedBillTable(QItemSelection)));
    QObject::connect(ui->tableViewTotalBill->selectionModel(),
                     SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(SelectedCellChangedHeaderTable(QItemSelection)));
}

void MainWindow::updateBillOptions()
{
    ui->comboBoxBMTaluka->clear();
    ui->comboBoxBMRout->clear();
    ui->comboBoxBMYear->clear();
    ui->comboBoxBMTaluka->addItem("All Taluka");

    for(int i=0;i<talukas->getTalukaList()->length();i++)
    {
        ui->comboBoxBMTaluka->addItem(talukas->getTalukaList()->at(i)->getTaluka());
    }

    ui->comboBoxBMRout->addItem("All Routs");
    for(int i=0; i<schools->getAllRouts()->length();i++){
        ui->comboBoxBMRout->addItem(schools->getAllRouts()->at(i));
    }

    QDate date = QDate::currentDate();
    for(int i=date.year()-10; i<date.year()+3; i++)
    {
        ui->comboBoxBMYear->addItem(QString::number(i));
    }

    ui->comboBoxBMYear->setCurrentText(QString::number(date.year()));

}

void MainWindow::ChooseDatabase()
{
    dirToDatabase =  QFileDialog::getExistingDirectory(this, "Select Folder","/",QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);

    if(!dirToDatabase.isEmpty()){
        db = new MainDatabase();
        db->Open(dirToDatabase);
        query = new QSqlQuery(*db->getDatabase());
    }
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
            if(userName == ui->lineEditUsername->text() && passWord == ui->lineEditPassword->text()){
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
    ui->actionAddUser->setEnabled(false);
    ui->actionAddAdmin->setEnabled(false);
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

void MainWindow::TalukaManagerEvent()
{
    TalukaManager *tm = new TalukaManager();
    tm->setWindowTitle("Taluka Manager");
    tm->open();
}

void MainWindow::SchoolManagerEvent()
{
    ui->AllStackWidget->setCurrentIndex(2);
    ui->groupBoxControl->setVisible(true);
    ResetSchool();
}

void MainWindow::SearchSchool(){
    sm->search(ui->lineEditSearchBox->text());
}

void MainWindow::setFilter(){
    sm->setFilter(ui->comboBoxSelectTaluka->currentText(),
                  ui->comboBoxSelectRout->currentText());
}

void MainWindow::ResetSchool(){
    ui->comboBoxSelectTaluka->clear();
    ui->comboBoxSelectTaluka->addItem("All Taluka");
    for(int i=0; i<talukas->getTalukaList()->length(); i++){
        ui->comboBoxSelectTaluka->addItem(talukas->getTalukaList()->at(i)->getTaluka());
    }
    ui->comboBoxSelectRout->clear();
    ui->comboBoxSelectRout->addItem("All Routs");
    for(int i=0; i<schools->getAllRouts()->length();i++){
        ui->comboBoxSelectRout->addItem(schools->getAllRouts()->at(i));
    }

    ui->lineEditSearchBox->setText("");
    ui->comboBoxSelectTaluka->setCurrentIndex(0);
    ui->comboBoxSelectRout->setCurrentIndex(0);

    sm->setUpTable();
}

void MainWindow::AddSchoolEvent()
{
    sm->addSchool();
}

void MainWindow::EditSchoolEvent()
{
    sm->editSchool();
}

void MainWindow::RemoveSchoolEvent()
{
    sm->removeSchool();
}

void MainWindow::PrintSchoolEvent(){
    if(ui->AllStackWidget->currentIndex() == 2)
    {
        fm->clearDocument();
        fm->writeTable(ui->tableViewSchools);
        fm->printTable();
    }else if(ui->AllStackWidget->currentIndex() == 3)
    {
        fm->clearDocument();
        fm->writeTable(ui->tableViewTotalBill);
        fm->writeTable(ui->tableViewMainBill);
        fm->printTable();
    }

}

void MainWindow::ExportSchoolEvent(){
    if(ui->AllStackWidget->currentIndex() == 2)
    {
        fm->setTable(ui->tableViewSchools);
        fm->exportCSV();
    }

}

void MainWindow::BackEvent()
{
    ui->AllStackWidget->setCurrentIndex(1);
    ui->groupBoxControl->setVisible(false);
}

void MainWindow::SchoolBillManagerEvent()
{
    ui->AllStackWidget->setCurrentIndex(3);
    ui->groupBoxControl->setVisible(true);

    ui->labelBMSTDBody->setText(ui->comboBoxBMSTD->currentText());
    ui->labelBMPeriodBody->setText(ui->comboBoxBMPeriod->currentText());
    ui->labelBMMonthBody->setText(ui->comboBoxBMMonth->currentText());
    ui->labelBMYearBody->setText(ui->comboBoxBMYear->currentText());

    sbm->navigatedToSBM(ui->comboBoxBMTaluka->currentText(),
                        ui->comboBoxBMRout->currentText(),
                        ui->comboBoxBMSTD->currentIndex(),
                        ui->comboBoxBMPeriod->currentIndex(),
                        ui->comboBoxBMMonth->currentText(),
                        ui->comboBoxBMYear->currentText());
}

void MainWindow::SelectedSchoolBillChanged(const QItemSelection& selection)
{
    sbm->schoolChanged();
}

void MainWindow::SelectedCellChangedBillTable(const QItemSelection& selection)
{
    sbm->SelectedCellChangedBillTable();
}

void MainWindow::SelectedCellChangedHeaderTable(const QItemSelection& selection)
{
    sbm->SelectedCellChangedHeaderTable();
}

void MainWindow::SaveSchoolBillEvent()
{
    sbm->SaveSchoolEvent();
}

void MainWindow::DeleteSchoolBillEvent()
{
    sbm->DeleteSchoolEvent();
}
