#include "talukadialog.h"
#include "ui_talukadialog.h"

TalukaDialog::TalukaDialog(QWidget *parent, int tid) :
    QDialog(parent),
    TID(tid), oldTaluka(""), oldDistrict(""),
    ui(new Ui::TalukaDialog)
{
    ui->setupUi(this);
    ui->labelError->setText("");

    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());

    talukas = AllTaluka::Instance();

    if(TID > -1){
        this->setUpFields();
    }
}

TalukaDialog::~TalukaDialog()
{
    delete ui;
}

void TalukaDialog::on_pushButtonSave_clicked()
{
    QString talukaName = ui->lineEditTaluka->text().replace(',',' ');
    QString districtName = ui->lineEditDistrict->text().replace(',',' ');
    if(this->Check()){
        if(TID == -1){
            talukas->getTalukaList()->append(new Taluka(talukaName, districtName));
        }else if(TID > -1){
            if(query->exec(db->getUpdateTalukaQuery( TID, talukaName, districtName))){
                talukas->removeTaluka(oldTaluka, oldDistrict);
                talukas->getTalukaList()->append(new Taluka(TID, talukaName, districtName));
            }else{
                qDebug() << "Taluka not updated";
            }
        }
        emit closed();
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

void TalukaDialog::setUpFields(){
    Taluka *t = talukas->getTaluka(TID);
    if(t != 0){
       oldTaluka = t->getTaluka();
       oldDistrict = t->getDistrict();
       ui->lineEditTaluka->setText(oldTaluka);
       ui->lineEditDistrict->setText(oldDistrict);
    }
}

void TalukaDialog::on_pushButtonCancle_clicked()
{
    emit closed();
    this->close();
}
