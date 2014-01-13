#include <QHeaderView>
#include <QAbstractItemView>
#include <QMessageBox>

#include "maindatabase.h"
#include "schoolmanager.h"
#include "schooldialog.h"


SchoolManager::SchoolManager(QTableView* table):
    table(table)
{
    schools = AllSchool::Instance();

    viewModel = new QStandardItemModel(0,0);
    sortModel = new QSortFilterProxyModel();
    sortModel->setSourceModel(viewModel);
    this->table->setModel(sortModel);
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->table->setSortingEnabled(true);
    this->table->horizontalHeader()->setStretchLastSection( true );
    this->setUpTable();

    instance = this;
}

SchoolManager* SchoolManager::instance = 0;

SchoolManager* SchoolManager::Instance(){
    return instance;
}

void SchoolManager::setUpTable(){
    viewModel->clear();

    viewModel->setHorizontalHeaderLabels( QStringList() << "Center No."
                                          << "School Name" << "Taluka"
                                          << "Rout No." << "Principal"
                                          << "Principal Mob no." << "Address" );

    QList<QStandardItem*> itemList;
    for(int i=0; i < schools->getSchoolList()->length(); i++){
        School* temp = schools->getSchoolList()->at(i);
        itemList << new QStandardItem(temp->CenterNo)
                 << new QStandardItem(temp->SchoolName)
                 << new QStandardItem(temp->Taluka)
                 << new QStandardItem(temp->RoutNo)
                 << new QStandardItem(temp->Principal)
                 << new QStandardItem(temp->PrincipalMobNo)
                 << new QStandardItem(temp->Address);
        viewModel->appendRow(itemList);
        itemList.clear();
    }
}

void SchoolManager::addSchool(){
    SchoolDialog *sc = new SchoolDialog();
    sc->setWindowTitle("Add School Information");
    sc->open();
}

void SchoolManager::editSchool(){
    int i = sortModel->mapToSource(this->table->currentIndex()).row();

    if(i < schools->getSchoolList()->length() && i > -1){
        SchoolDialog *sc = new SchoolDialog(0, schools->getSchoolList()->at(i)->SID);
        sc->setWindowTitle("Update School Information");
        sc->open();
    }else{
        QMessageBox::information(0,"Select School","Please Select School to Edit",0,0);
    }
}

void SchoolManager::removeSchool(){
    int i = sortModel->mapToSource(this->table->currentIndex()).row();

    if(i<schools->getSchoolList()->length() && i > -1){
        MainDatabase *db = MainDatabase::Instance();
        QSqlQuery *query = new QSqlQuery(*db->getDatabase());
        if(query->exec(db->getDeleteSchoolQuery(schools->getSchoolList()->at(i)->CenterNo,
                                                schools->getSchoolList()->at(i)->Taluka))){
            schools->getSchoolList()->removeAt(i);
            this->setUpTable();
        }else{
            qDebug() << "Could not remove school";
        }
    }else{
        QMessageBox::information(0,"Select School","Please Select School to Delete",0,0);
    }
}
