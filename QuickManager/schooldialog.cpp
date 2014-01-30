#include "schooldialog.h"
#include "ui_schooldialog.h"
#include "DataManager/schoolmanager.h"

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

    if(SID > -1){
        setUpFields(schools->getSchoolList()->at(schools->indexOfSchoolWith(SID)));
    }
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

void SchoolDialog::setUpFields(School* school){
    ui->lineEditCenterNo->setText(school->CenterNo);
    ui->comboBoxTaluka->setCurrentText(school->Taluka);
    ui->lineEditSchoolName->setText(school->SchoolName);
    ui->textEditSchoolAddress->setText(school->Address);
    ui->lineEditPrincipal->setText(school->Principal);
    ui->lineEditPrincipalMobNo->setText(school->PrincipalMobNo);
    ui->lineEditPrincipalLandNo->setText(school->PrincipalLandline);
    ui->lineEditTeacherName->setText(school->Teacher);
    ui->lineEditTeacherMobNo->setText(school->TeacherMobNo);
    ui->textEditPrincipalAddress->setText(school->PrincipalAddress);
    ui->lineEditRoutNo->setText(school->RoutNo);
    ui->comboBoxGender->setCurrentText(school->girlMIXboy);
    ui->comboBoxType->setCurrentText(school->Type);
    ui->comboBoxStd->setCurrentText(school->isPrimary);
}

void SchoolDialog::on_pushButtonSave_clicked()
{
    if(Check()){
        if(SID == -1){
            schools->addSchool(new School(CenterNo,Taluka,SchoolName,
                                                        Address,Principal,PrincipalMobNo,PrincipalLandline,
                                                        Teacher,TeacherMobNo,PrincipalAddress,RoutNo,girlMIXboy,
                                                        Type,isPrimary));
        }else if(SID > -1){
            if(query->exec(db->getUpdateSchoolQuery(SID,CenterNo,Taluka,SchoolName,
                                                    Address,Principal,PrincipalMobNo,PrincipalLandline,
                                                    Teacher,TeacherMobNo,PrincipalAddress,RoutNo,girlMIXboy,
                                                    Type,isPrimary))){
                schools->getSchoolList()->removeAt(schools->indexOfSchoolWith(SID));
                schools->addSchool(new School(SID,CenterNo,Taluka,SchoolName,
                                                            Address,Principal,PrincipalMobNo,PrincipalLandline,
                                                            Teacher,TeacherMobNo,PrincipalAddress,RoutNo,girlMIXboy,
                                                            Type,isPrimary));

            }else{
                qDebug() << "Could not update school";
            }
        }
        SchoolManager::Instance()->setUpTable();
        this->close();
    }
}

void SchoolDialog::on_pushButtonCancle_clicked()
{
    this->close();
}
