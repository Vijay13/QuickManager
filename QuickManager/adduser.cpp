#include "adduser.h"
#include "ui_adduser.h"

AddUser::AddUser(QWidget *parent, bool isuser) :
    QDialog(parent),
    ui(new Ui::AddUser),
    isUser(isuser)
{
    ui->setupUi(this);
    ui->labelError->setText("");
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditRePassword->setEchoMode(QLineEdit::Password);

    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_pushButtonSave_clicked()
{
    if(this->Check()){
        if(query->exec(db->getInsertUserQuery(ui->lineEditUsername->text(), ui->lineEditPassword->text(), !isUser))){
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

void AddUser::clearFields(){
    ui->lineEditUsername->setText("");
    ui->lineEditPassword->setText("");
    ui->lineEditRePassword->setText("");
    ui->labelError->setText("");
}
