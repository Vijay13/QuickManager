#include "foodlistmanager.h"

#include <QHeaderView>

FoodListManager::FoodListManager(QTableView *tableFoodList) : table(tableFoodList)
{
    schools = AllSchool::Instance();
    viewModel = new QStandardItemModel();
    sortModel = new QSortFilterProxyModel();
    sortModel->setSourceModel(viewModel);

    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());

    setUpHeaderTable();
}

void FoodListManager::setUpHeaderTable(){

    viewModel = new QStandardItemModel(0,17);
    table->setModel(viewModel);
    viewModel->setHorizontalHeaderLabels( QStringList()
                                                     << "Centre No" << "School Name"
                                                     << "Std 1-5" << "Std 6-8"
                                                     << "Roti-B" << "Roti-M"
                                                     << "Roti-S" << "Batch-No"
                                                     << "Rice-B" << "Rice-M"
                                                    << "Rice-S" << "Batch-No"
                                                    << "Sabji-B" << "Sabji-M"
                                                    << "Sabji-S" << "Batch-No"
                                                    << "Total");
}

void FoodListManager::setUpTables(QString rout){

    for(int i = 0; i < schools->getSchoolList()->length(); i++){
        if(schools->getSchoolList()->at(i)->RoutNo == rout){
            viewModel->setItem(i,0,new QStandardItem(schools->getSchoolList()->at(i)->CenterNo));
            viewModel->setItem(i,1,new QStandardItem(schools->getSchoolList()->at(i)->SchoolName));
        }
    }

}
