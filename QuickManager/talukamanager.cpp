#include "talukamanager.h"
#include "ui_talukamanager.h"

#include <QMessageBox>
#include "talukadialog.h"

TalukaManager::TalukaManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TalukaManager)
{
    ui->setupUi(this);
    table = ui->tableViewTalukas;

    this->initialize();
    this->setUpTable();

    connect(ui->pushButtonAddTaluka,SIGNAL(clicked()),this,SLOT(addTaluka()));
    connect(ui->pushButtonEditTaluka,SIGNAL(clicked()),this,SLOT(editTaluka()));
    connect(ui->pushButtonRemoveTaluka,SIGNAL(clicked()),this,SLOT(removeTaluka()));
    connect(ui->pushButtonClose,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pushButtonPrintTaluka,SIGNAL(clicked()),this,SLOT(printTaluka()));
    connect(ui->pushButtonExportTaluka,SIGNAL(clicked()),this,SLOT(exportTaluka()));
}

TalukaManager::~TalukaManager()
{
    delete ui;
}

void TalukaManager::initialize(){
    talukas = AllTaluka::Instance();
    fm = FileManager::Instance();

    viewModel = new QStandardItemModel(0,0);
    sortModel = new QSortFilterProxyModel();
    sortModel->setSourceModel(viewModel);
    this->table->setModel(sortModel);
    this->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->table->setSortingEnabled(true);
    this->table->horizontalHeader()->setStretchLastSection( true );
    this->setUpTable();
}

void TalukaManager::setUpTable(){
    viewModel->clear();

    viewModel->setHorizontalHeaderLabels( QStringList() << "Sr.No."
                                          << "Taluka"
                                          << "District" );

    QList<QStandardItem*> itemList;
    for(int i=0; i < talukas->getTalukaList()->length(); i++){
        Taluka* temp = talukas->getTalukaList()->at(i);
        itemList << new QStandardItem(QString::number(i+1))
                 << new QStandardItem(temp->getTaluka())
                 << new QStandardItem(temp->getDistrict());
        viewModel->appendRow(itemList);
        itemList.clear();
    }

    fm->setTable(table);
}

void TalukaManager::setUpControl(int TMAdd, int TMUpdate, int TMDelete)
{
    if(TMAdd == 0)
        ui->pushButtonAddTaluka->setEnabled(false);
    else
        ui->pushButtonAddTaluka->setEnabled(true);

    if(TMUpdate == 0)
        ui->pushButtonEditTaluka->setEnabled(false);
    else
        ui->pushButtonEditTaluka->setEnabled(true);

    if(TMDelete == 0)
        ui->pushButtonRemoveTaluka->setEnabled(false);
    else
        ui->pushButtonRemoveTaluka->setEnabled(true);
}

void TalukaManager::addTaluka(){
    TalukaDialog *t = new TalukaDialog();
    t->setWindowTitle("Add Taluka Information");
    t->open();
    connect(t,SIGNAL(closed()),this,SLOT(updateTableView()));
}

void TalukaManager::editTaluka(){
    int i = sortModel->mapToSource(this->table->currentIndex()).row();

    if(i < talukas->getTalukaList()->length() && i > -1){
        TalukaDialog *t = new TalukaDialog(0,talukas->getTalukaList()->at(i)->getTID());
        t->setWindowTitle("Update Taluka Information");
        t->open();
        connect(t,SIGNAL(closed()),this,SLOT(updateTableView()));
    }else{
        QMessageBox::information(0,"Select Taluka","Please Select Taluka to Edit",0,0);
    }
}

void TalukaManager::removeTaluka(){
    int i = sortModel->mapToSource(this->table->currentIndex()).row();

    if(i < talukas->getTalukaList()->length() && i > -1){
        talukas->removeTaluka(talukas->getTalukaList()->at(i)->getTaluka(),
                              talukas->getTalukaList()->at(i)->getDistrict());
    }else{
        QMessageBox::information(0,"Select Taluka","Please Select Taluka to Edit",0,0);
    }
    this->setUpTable();
}

void TalukaManager::printTaluka(){
    fm->printTable();
}

void TalukaManager::exportTaluka(){
    fm->exportCSV();
}
void TalukaManager::updateTableView(){
    this->setUpTable();
}
