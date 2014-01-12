#include "schooldialog.h"
#include "ui_schooldialog.h"

SchoolDialog::SchoolDialog(QWidget *parent, int sid):
    QDialog(parent),
    SID(sid),
    ui(new Ui::SchoolDialog)
{
    ui->setupUi(this);

    this->Initialize();
    this->initializeComponent();
}

SchoolDialog::~SchoolDialog()
{
    delete ui;
}

void SchoolDialog::initializeComponent(){
    for(int i=0; i<talukas->getTalukaList()->size(); i++){
        ui->comboBoxTaluka->addItem(talukas->getTalukaList()->at(i)->getTaluka());
    }

    ui->labelError->setText("");
}

void SchoolDialog::Initialize(){
    talukas = AllTaluka::Instance();
    schools = AllSchool::Instance();
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());
}

bool SchoolDialog::Check(){
    if(ui->lineEditCenterNo->text().isEmpty()){
        ui->labelError->setText("Please enter Center Number");
        return false;
    }else if(ui->lineEditSchoolName->text().isEmpty()){
        ui->labelError->setText("Please enter School Name");
        return false;
    }else if(ui->lineEditRoutNo->text().isEmpty()){
        ui->labelError->setText("Please enter Rout Number");
        return false;
    }else if(talukas->getTalukaList()->size() <= 0){
        ui->labelError->setText("Please Add Taluka First");
        return false;
    }

    CenterNo = ui->lineEditCenterNo->text();
    Taluka = ui->comboBoxTaluka->currentText();
    SchoolName = ui->lineEditSchoolName->text();
    Address = ui->textEditSchoolAddress->toPlainText();
    Principal = ui->lineEditPrincipal->text();
    PrincipalMobNo = ui->lineEditPrincipalMobNo->text();
    PrincipalLandline = ui->lineEditPrincipalLandNo->text();
    Teacher = ui->lineEditTeacherName->text();
    TeacherMobNo = ui->lineEditTeacherMobNo->text();
    PrincipalAddress = ui->textEditPrincipalAddress->toPlainText();
    RoutNo = ui->lineEditRoutNo->text();
    girlMIXboy = ui->comboBoxGender->currentText();
    Type = ui->comboBoxType->currentText();
    isPrimary = ui->comboBoxStd->currentText();

    return true;
}

void SchoolDialog::on_pushButtonSave_clicked()
{
    if(Check() && SID == -1){
        schools->addSchool(new School(CenterNo,Taluka,SchoolName,
                                                    Address,Principal,PrincipalMobNo,PrincipalLandline,
                                                    Teacher,TeacherMobNo,PrincipalAddress,RoutNo,girlMIXboy,
                                                    Type,isPrimary));
        this->close();
    }
}

void SchoolDialog::on_pushButtonCancle_clicked()
{
    this->close();
}
