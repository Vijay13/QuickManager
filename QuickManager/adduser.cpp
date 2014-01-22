#include "adduser.h"
#include "ui_adduser.h"

AddUser::AddUser(QWidget *parent, int isAdmin) :
    QDialog(parent),
    ui(new Ui::AddUser),
    isAdmin(isAdmin),
    TMAdd(1),TMUpdate(1),TMDelete(1),
    SMAdd(1),SMUpdate(1),SMDelete(1),
    SBMAdd(1),SBMUpdate(1),SBMDelete(1)
{
    ui->setupUi(this);
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditRePassword->setEchoMode(QLineEdit::Password);

    clearFields();
    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_pushButtonSave_clicked()
{
    setChecksValue();
    if(this->Check()){
        if(query->exec(db->getInsertUserQuery(ui->lineEditUsername->text(), ui->lineEditPassword->text(),
                                              isAdmin, TMAdd, TMUpdate, TMDelete,
                                              SMAdd,SMUpdate, SMDelete,
                                              SBMAdd, SBMUpdate, SBMDelete))){
            this->close();
        }else{
            qDebug() << "Could not save user";
        }
    }
}

bool AddUser::Check(){
    if(ui->lineEditUsername->text().isEmpty()){
        ui->labelError->setText("*Please Enter Username");
        return false;
    }else if(ui->lineEditPassword->text().isEmpty()){
        ui->labelError->setText("*Please Choose Password");
        return false;
    }else if(ui->lineEditRePassword->text().isEmpty()){
        ui->labelError->setText("*Please Repeate Password");
        return false;
    }

    if(ui->lineEditPassword->text() != ui->lineEditRePassword->text()){
        ui->labelError->setText("*Password did not Match");
        return false;
    }

    return true;
}

void AddUser::on_pushButtonReset_clicked()
{
    this->clearFields();
}

void AddUser::on_pushButtonCancle_clicked()
{
    this->clearFields();
    this->close();
}

void AddUser::setChecksValue()
{
    if(!ui->checkBoxTMAdd->isChecked())
        TMAdd = 0;
    if(!ui->checkBoxTMUpdate->isChecked())
        TMUpdate = 0;
    if(!ui->checkBoxTMDelete->isChecked())
        TMDelete = 0;
    if(!ui->checkBoxSMAdd->isChecked())
        SMAdd = 0;
    if(!ui->checkBoxSMUpdate->isChecked())
        SMUpdate = 0;
    if(!ui->checkBoxSMDelete->isChecked())
        SMDelete = 0;
    if(!ui->checkBoxSBMAdd->isChecked())
        SBMAdd = 0;
    if(!ui->checkBoxSBMUpdate->isChecked())
        SBMUpdate = 0;
    if(!ui->checkBoxSBMDelete->isChecked())
        SBMDelete = 0;
}

void AddUser::clearFields(){
    ui->lineEditUsername->setText("");
    ui->lineEditPassword->setText("");
    ui->lineEditRePassword->setText("");
    ui->labelError->setText("");

    ui->checkBoxTMAdd->setChecked(true);
    ui->checkBoxTMUpdate->setChecked(true);
    ui->checkBoxTMDelete->setChecked(true);
    ui->checkBoxSMAdd->setChecked(true);
    ui->checkBoxSMUpdate->setChecked(true);
    ui->checkBoxSMDelete->setChecked(true);
    ui->checkBoxSBMAdd->setChecked(true);
    ui->checkBoxSBMUpdate->setChecked(true);
    ui->checkBoxSBMDelete->setChecked(true);

    TMAdd = 1;
    TMUpdate = 1;
    TMDelete = 1;
    SMAdd = 1;
    SMUpdate = 1;
    SMDelete = 1;
    SBMAdd = 1;
    SBMUpdate = 1;
    SBMDelete = 1;
}

