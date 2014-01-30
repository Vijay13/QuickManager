
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
    isCorrect(false), queryStatement(""), dirToDatabase(""),
    TMAdd(1),TMUpdate(1),TMDelete(1)
{
    ui->setupUi(this);

    initialize();
    initializeComponent();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    db = new MainDatabase();
    db->Open(dirToDatabase);
    query = new QSqlQuery(*db->getDatabase());

    fm = new FileManager();
    talukas = new AllTaluka();
    schools = new AllSchool();
    sm = new SchoolManager(ui->tableViewSchools);
    sbm = new SchoolBillManager(ui->pushButtonEditSchoolBill, ui->pushButtonDeleteSchoolBill,
                                ui->progressBar,ui->listViewSchools ,ui->tableViewTotalBill ,
                                ui->tableViewMainBillAttendence,
                                ui->tableViewCheckTableAttendence,
                                ui->tableViewMainBillBeneficiaries,
                                ui->tableViewCheckTableBeneficiaries,
                                ui->labelBMTalukaBody, ui->labelBMSchoolBody);
}

void MainWindow::initializeComponent(){
    this->setWindowTitle("Quick Manager Akshaypatra");
    ui->ErrorLable->setText("");
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->actionAddUser->setDisabled(true);
    ui->actionAddAdmin->setDisabled(true);
    ui->AllStackWidget->setCurrentIndex(1);
    ui->progressBar->setVisible(false);

    this->setVisiblity(false);

    this->updateBillOptions();

    QObject::connect(ui->actionChooseDatabase,SIGNAL(triggered()),this,SLOT( ChooseDatabaseEvent()));
    QObject::connect(ui->pushButtonLogin,SIGNAL(clicked()),this,SLOT( LoginEvent()));
    QObject::connect(ui->actionLogout,SIGNAL(triggered()),this,SLOT( LogoutEvent()));
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

    QObject::connect(ui->lineEditSearchBox,SIGNAL(textChanged(const QString &)), SLOT( SearchSchoolEvent()) );
    QObject::connect(ui->comboBoxSelectTaluka,SIGNAL(activated(int)), SLOT( setFilterEvent()) );
    QObject::connect(ui->comboBoxSelectRout,SIGNAL(activated(int)), SLOT( setFilterEvent()) );
    QObject::connect(ui->pushButtonResetSchoolSearch,SIGNAL(clicked()), SLOT( ResetSchoolEvent()) );

    QObject::connect(ui->pushButtonBMManager,SIGNAL(clicked()), SLOT( SchoolBillManagerEvent()) );
    QObject::connect(ui->lineEditBMCenter,SIGNAL(textChanged(QString)),this,SLOT(SchoolBillCenterFilterEvent()));
    QObject::connect(ui->pushButtonSaveSchoolBill,SIGNAL(clicked()),SLOT(SaveSchoolBillEvent()));
    QObject::connect(ui->pushButtonDeleteSchoolBill,SIGNAL(clicked()),SLOT(DeleteSchoolBillEvent()));
    QObject::connect(ui->pushButtonEditSchoolBill, SIGNAL(clicked()), SLOT(EditSchoolBillEvent()));

    QObject::connect(ui->listViewSchools->selectionModel(),
                     SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(SelectedSchoolBillChanged(QItemSelection)));
    QObject::connect(ui->tableViewTotalBill->selectionModel(),
                     SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(SelectedCellChangedHeaderTable(QItemSelection)));
    QObject::connect(ui->tableViewMainBillAttendence->selectionModel(),
                     SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(SelectedCellChangedAttendenceTable(QItemSelection)));
    QObject::connect(ui->tableViewMainBillBeneficiaries->selectionModel(),
                     SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(SelectedCellChangedBeneficiariesTable(QItemSelection)));
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

void MainWindow::setUpControl(int TMAdd,int TMUpdate,int TMDelete,
                              int SMAdd,int SMUpdate,int SMDelete,
                              int SBMAdd, int SBMUpdate,int SBMDelete)
{
    this->TMAdd = TMAdd;
    this->TMUpdate = TMUpdate;
    this->TMDelete = TMDelete;

    if(SMAdd == 0)
        ui->pushButtonAddSchool->setEnabled(false);
    else
        ui->pushButtonAddSchool->setEnabled(true);

    if(SMUpdate == 0)
        ui->pushButtonEditSchool->setEnabled(false);
    else
        ui->pushButtonEditSchool->setEnabled(true);

    if(SMDelete == 0)
        ui->pushButtonRemoveSchool->setEnabled(false);
    else
        ui->pushButtonRemoveSchool->setEnabled(true);

    if(SBMUpdate == 0 || SBMAdd == 0)
        ui->pushButtonSaveSchoolBill->setEnabled(false);
    else
        ui->pushButtonSaveSchoolBill->setEnabled(true);

    if(SBMDelete == 0)
        ui->pushButtonDeleteSchoolBill->setEnabled(false);
    else
        ui->pushButtonDeleteSchoolBill->setEnabled(true);
}

void MainWindow::ChooseDatabaseEvent()
{
    dirToDatabase =  QFileDialog::getExistingDirectory(this, "Select Folder","/",QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);

    if(!dirToDatabase.isEmpty()){
        initialize();
        updateBillOptions();
    }
}

void MainWindow::LoginEvent()
{
    int isAdmin = ui->comboBoxUserAdmin->currentIndex();
    isCorrect = false;
    queryStatement = "SELECT * FROM USERS WHERE isAdmin = " + QString::number(isAdmin);

    if(query->exec(queryStatement)){
        while(query->next()){
            QString userName = query->value("UserName").toString();
            QString passWord = query->value("Password").toString();
            if(userName == ui->lineEditUsername->text() && passWord == ui->lineEditPassword->text()){
                isCorrect = true;
                setUpControl(query->value("TMAdd").toInt(),
                             query->value("TMUpdate").toInt(),
                             query->value("TMDelete").toInt(),
                             query->value("SMAdd").toInt(),
                             query->value("SMUpdate").toInt(),
                             query->value("SMDelete").toInt(),
                             query->value("SBMAdd").toInt(),
                             query->value("SBMUpdate").toInt(),
                             query->value("SBMDelete").toInt());
                break;
            }
        }
    }

    if(isCorrect){
        ui->AllStackWidget->setCurrentIndex(1);
        if(isAdmin){
            ui->actionChooseDatabase->setEnabled(false);
            ui->actionAddUser->setEnabled(true);
            ui->actionAddAdmin->setEnabled(true);
        }
        this->setWindowTitle("Quick Manager Akshaypatra - " + query->value("UserName").toString());
    }else{
        ui->ErrorLable->setText("Incorrect Username or Password");
    }
    query->finish();
}

void MainWindow::LogoutEvent()
{
    ui->AllStackWidget->setCurrentIndex(0);
    ui->lineEditUsername->setText("");
    ui->lineEditPassword->setText("");
    ui->ErrorLable->setText("");
    ui->actionAddUser->setEnabled(false);
    ui->actionAddAdmin->setEnabled(false);
    this->setWindowTitle("Quick Manager Akshaypatra");
}

void MainWindow::AddUserEvent()
{
    AddUser *addUser = new AddUser( this, 0);
    addUser->setWindowTitle("Add User");
    addUser->open();
}

void MainWindow::AddAdminEvent()
{
    AddUser *addUser = new AddUser( this, 1);
    addUser->setWindowTitle("Add Admin");
    addUser->open();
}

void MainWindow::TalukaManagerEvent()
{
    TalukaManager *tm = new TalukaManager();
    tm->setWindowTitle("Taluka Manager");
    tm->setUpControl(this->TMAdd, this->TMUpdate, this->TMDelete);
    tm->open();
}

void MainWindow::SchoolManagerEvent()
{
    ui->AllStackWidget->setCurrentIndex(2);
    this->setVisiblity(true);
    ResetSchoolEvent();
}

void MainWindow::SearchSchoolEvent(){
    sm->search(ui->lineEditSearchBox->text());
}

void MainWindow::setFilterEvent(){
    sm->setFilter(ui->comboBoxSelectTaluka->currentText(),
                  ui->comboBoxSelectRout->currentText());
}

void MainWindow::ResetSchoolEvent(){
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
        fm->writeLine("Month: " + sbm->currentMonth +
                      this->getSpace(20) +"Year: " + sbm->currentYear +
                      this->getSpace(20) +"STD: " + sbm->currentSTD);
        fm->writeLine("School: " + sbm->currentSchool +
                      this->getSpace(20) +"Taluka: " + sbm->currentTaluka +
                      this->getSpace(20) +"Center No.: " + sbm->currentCenter);
        fm->writeTable(ui->tableViewTotalBill);
        fm->writeHeader("ATTENDENCE");
        fm->writeTable(ui->tableViewMainBillAttendence);
        fm->writeHeader("BENEFICIARIES");
        fm->writeTable(ui->tableViewMainBillBeneficiaries);
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
    this->setVisiblity(false);
}

void MainWindow::SchoolBillManagerEvent()
{
    ui->AllStackWidget->setCurrentIndex(3);
    this->setVisiblity(true);

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

void MainWindow::SelectedCellChangedAttendenceTable(const QItemSelection& selection)
{
    sbm->SelectedCellChangedMainBillTable(ui->tableViewMainBillAttendence);
}

void MainWindow::SelectedCellChangedBeneficiariesTable(const QItemSelection& selection)
{
    sbm->SelectedCellChangedMainBillTable(ui->tableViewMainBillBeneficiaries);
}

void MainWindow::SelectedCellChangedHeaderTable(const QItemSelection& selection)
{
    sbm->SelectedCellChangedHeaderTable();
}

void MainWindow::SchoolBillCenterFilterEvent()
{
    sbm->SchoolCenterNameChanged(ui->lineEditBMCenter->text());
}


void MainWindow::EditSchoolBillEvent()
{
    sbm->EditSchoolEvent();
}

void MainWindow::SaveSchoolBillEvent()
{
    sbm->SaveSchoolEvent();
}

void MainWindow::DeleteSchoolBillEvent()
{
    sbm->DeleteSchoolEvent();
}

void MainWindow::setVisiblity(bool visible)
{
    ui->pushButtonPrintSchools->setVisible(visible);
    ui->pushButtonExportSchools->setVisible(visible);
    ui->progressBar->setVisible(visible);
}

//private methods
QString MainWindow::getSpace(int count)
{
    QString space;
    for(int i=0;i<count;i++)
        space  += "&nbsp;";
    return space;
}
