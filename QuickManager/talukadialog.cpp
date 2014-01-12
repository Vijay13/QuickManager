#include "talukadialog.h"
#include "ui_talukadialog.h"

TalukaDialog::TalukaDialog(QWidget *parent, int tid) :
    QDialog(parent),
    TID(tid),
    ui(new Ui::TalukaDialog)
{
    ui->setupUi(this);
    ui->labelError->setText("");

    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());

    talukas = AllTaluka::Instance();
    oldTaluka = ui->lineEditTaluka->text();
    oldDistrict = ui->lineEditDistrict->text();
}

TalukaDialog::~TalukaDialog()
{
    delete ui;
}

void TalukaDialog::on_pushButtonSave_clicked()
{
    if(this->Check()){
        if(TID == -1){
            talukas->getTalukaList()->append(new Taluka(ui->lineEditTaluka->text(), ui->lineEditDistrict->text()));
        }else if(TID > -1){
            talukas->removeTaluka(oldTaluka, oldDistrict);
            talukas->getTalukaList()->append(new Taluka(TID, ui->lineEditTaluka->text(), ui->lineEditDistrict->text()));
        }
        this->close();
    }
}

bool TalukaDialog::Check(){
    if(ui->lineEditTaluka->text().isEmpty()){
        ui->labelError->setText("Please enter Taluka name");
        return false;
    }else if(ui->lineEditDistrict->text().isEmpty()){
        ui->labelError->setText("Please enter District name");
        return false;
    }

    return true;
}

void TalukaDialog::on_pushButtonCancle_clicked()
{
    this->close();
}
