#include "foodlistmanager.h"

#include <QHeaderView>

FoodListManager::FoodListManager(QTableView *tableFoodList, QProgressBar *progressBar) : table(tableFoodList), progressBar(progressBar)
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

void FoodListManager::setUpTables(QString date, QString rout){
    this->date = date;
    this->rout = rout;

    viewModel->clear();
    setUpHeaderTable();

    fillDataInTable();
}

void FoodListManager::saveFoodListEvent(){
    if(query->exec(db->getCreateFoodListTable(this->getTableName()))){
        insertDataToTable(this->getTableName(),this->table);
    }else{
        qDebug() << "Could not create FoodList Table: " << this->getTableName();
    }
}

void FoodListManager::insertDataToTable(QString tableName, QTableView *table){
    int progress = 0;
    progressBar->setValue(progress);
    progressBar->setVisible(true);

    for(int row=0; row < table->model()->rowCount(); row++)
    {
        if(query->exec(db->getInsertFoodList(tableName,
                                             table->model()->data(table->model()->index(row,0)).toString(),
                                             table->model()->data(table->model()->index(row,1)).toString(),
                                             dataInTable(table, row, 2),
                                             dataInTable(table, row, 3),
                                             dataInTable(table, row, 4),
                                             dataInTable(table, row, 5),
                                             dataInTable(table, row, 6),
                                             dataInTable(table, row, 7),
                                             dataInTable(table, row, 8),
                                             dataInTable(table, row, 9),
                                             dataInTable(table, row, 10),
                                             dataInTable(table, row, 11),
                                             dataInTable(table, row, 12),
                                             dataInTable(table, row, 13),
                                             dataInTable(table, row, 14),
                                             dataInTable(table, row, 15),
                                             dataInTable(table, row, 16)))){

        }else{
            qDebug() << "Data not inserted in FoodList Table";
        }
        query->finish();
        progress += 7; // 100/15 ~ 7
        progressBar->setValue(progress);
    }

    progressBar->setVisible(false);
}

void FoodListManager::fillDataInTable(){
    if(query->exec(db->getFoodListTable(this->getTableName()))){
        int row = 0;
        while(query->next()){
            for(int column=0; column<17; column++)
            {
                viewModel->setItem(row,column,new QStandardItem(query->value(column).toString()));
            }
            row++;
        }
    }else{
        for(int i = 0; i < schools->getSchoolList()->length(); i++){
            if(schools->getSchoolList()->at(i)->RoutNo == rout){
                viewModel->setItem(i,0,new QStandardItem(schools->getSchoolList()->at(i)->CenterNo));
                viewModel->setItem(i,1,new QStandardItem(schools->getSchoolList()->at(i)->SchoolName));
            }
        }
    }
    query->finish();
}

void FoodListManager::deleteFoodListEvent(){
    if(query->exec(db->getDeleteFoodListTable(this->getTableName()))){

    }else{
        qDebug() << "Could not delete FoodList Table";
    }
}

QString FoodListManager::getTableName(){
    return "FoodList_" + date + "_" + rout.remove(' ');
}

int FoodListManager::dataInTable(QTableView *tempTable, int r, int c) const
{
    bool ok;
    tempTable->model()->data(tempTable->model()->index(r,c)).toInt(&ok);
    if(ok)
        return tempTable->model()->data(tempTable->model()->index(r,c)).toInt();
    else
        return 0;
}
