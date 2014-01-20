#include "schoolbillmanager.h"
#include <QHeaderView>
#include <QMessageBox>

SchoolBillManager::SchoolBillManager(QProgressBar* progressBar,QListView* list,
                                     QTableView* headerTable, QTableView* table,
                                     QLabel* center, QLabel* school):
    progressBar(progressBar), headerTable(headerTable),table(table),
    list(list), centerNo(center), schoolName(school)
{
    schools = AllSchool::Instance();
    viewModelList = new QStandardItemModel();
    sortModelList = new QSortFilterProxyModel();
    sortModelList->setSourceModel(viewModelList);
    this->list->setModel(sortModelList);

    db = MainDatabase::Instance();
    query = new QSqlQuery(*db->getDatabase());

    setUpHeaderTable();
    setUpTable();
    //this->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

SchoolBillManager* SchoolBillManager::instance = 0;

SchoolBillManager* SchoolBillManager::Instance()
{
    return instance;
}

void SchoolBillManager::setUpHeaderTable()
{
    viewModelHeaderTable = new QStandardItemModel(1,11);
    headerTable->setModel(viewModelHeaderTable);
    viewModelHeaderTable->setHorizontalHeaderLabels( QStringList()
                                                     << "SC-B" << "SC-G"
                                                     << "ST-B" << "SC-G"
                                                     << "OBC-B" << "OBC-G"
                                                     << "Gen-B" << "Gen-G"
                                                     << "Total-B" << "Total-G"
                                                     << "Total");
    viewModelHeaderTable->setItem(0,8,new QStandardItem("0"));
    viewModelHeaderTable->setItem(0,9,new QStandardItem("0"));
    viewModelHeaderTable->setItem(0,10,new QStandardItem("0"));
}

void SchoolBillManager::setUpTable()
{
    viewModelTable = new QStandardItemModel(15,12);
    sortModelTable = new QSortFilterProxyModel();
    sortModelTable->setSourceModel(viewModelTable);
    table->setModel(sortModelTable);
    viewModelTable->setHorizontalHeaderLabels( QStringList() << "Date"
                                               << "SC-B" << "SC-G"
                                               << "ST-B" << "SC-G"
                                               << "OBC-B" << "OBC-G"
                                               << "Gen-B" << "Gen-G"
                                               << "Total-B" << "Total-G"
                                               << "Total");
    setUpZeroinMainTable();
}

void SchoolBillManager::setDates()
{
    int start;
    if(currentPeriod == "1to15")
        start = 0;
    else
        start = 15;

    for(int i=0; i < 15 ; i++)
    {
        viewModelTable->setItem(i,0,new QStandardItem(QString::number(++start)));
    }
}

void SchoolBillManager::setUpZeroinMainTable()
{
    previousRow = -1;
    previousColumn = -1;
    for(int row=0; row<15; row++)
    {
        for(int column=9; column<12; column++)
            viewModelTable->setItem(row,column,new QStandardItem("0"));
    }
}

void SchoolBillManager::navigatedToSBM(QString taluka, QString rout, int std, int period,
                                       QString month, QString year)
{
    if(std == 0)
        currentSTD = "1to5";
    else
        currentSTD = "6th8";

    if(period == 0)
        currentPeriod = "1to15";
    else
        currentPeriod = "16to31";

    currentMonth = month;
    currentYear = year;
    currentSchool = "";

    setDates();

    viewModelList->clear();
    for(int i=0 ; i< schools->getSchoolList()->length() ; i++)
    {
        if(!taluka.contains("All Taluka",Qt::CaseInsensitive))
        {
            if(taluka != schools->getSchoolList()->at(i)->Taluka)
                continue;
        }

        if(!rout.contains("All Rout",Qt::CaseInsensitive))
        {
            if(rout != schools->getSchoolList()->at(i)->RoutNo)
                continue;
        }

        viewModelList->appendRow(new QStandardItem(schools->getSchoolList()->at(i)->SchoolName));
    }

}

void SchoolBillManager::schoolChanged()
{
    School* tempSchool = schools->getSchool(list->currentIndex().data(Qt::DisplayRole).toString());

    if(tempSchool != 0)
    {
        currentSchool = tempSchool->SchoolName;
        centerNo->setText(tempSchool->CenterNo);
        schoolName->setText(currentSchool);
    }
}

bool SchoolBillManager::checkData()
{
    int row,column;

    for(int i=0; i<15; i++)
    {
        if(dataInTable(table,i,0) < 1 || dataInTable(table,i,0) > 31)
        {
            QMessageBox::information(0,"Invalid Date","Date can not be " + QString::number(dataInTable(table,i,0))
                                     + " at " + QString::number(i) ,0,0);
            return false;
        }
    }
    for(row=0; row < 15; row++)
    {
        for(column=1; column < 12; column ++)
        {
            if(!(dataInTable(table,row,column) <= dataInTable(headerTable,0,column-1)))
            {
                qDebug() << "Not fair at: " << row << column;
            }
        }
    }

    return true;
}

int SchoolBillManager::dataInTable(QTableView *tempTable, int r, int c)
{
    bool ok;
    tempTable->model()->data(tempTable->model()->index(r,c)).toInt(&ok);
    if(ok)
        return tempTable->model()->data(tempTable->model()->index(r,c)).toInt();
    else
        return 0;
}

void SchoolBillManager::SelectedCellChangedBillTable()
{
    int sum=0,row = table->currentIndex().row();

    if(previousRow != row && previousRow != -1)
    {
        // counting boys
        for(int i=1;i<9;i+=2)
        {
            sum += dataInTable(table,previousRow,i);
        }
        viewModelTable->setItem(previousRow,9,new QStandardItem(QString::number(sum)));

        //counting girls
        sum = 0;
        for(int i=2;i<9;i+=2)
        {
            sum += dataInTable(table,previousRow,i);
        }
        viewModelTable->setItem(previousRow,10,new QStandardItem(QString::number(sum)));

        //counting total strength
        sum = dataInTable(table,previousRow,9) + dataInTable(table,previousRow,10);
        viewModelTable->setItem(previousRow,11,new QStandardItem(QString::number(sum)));
    }
    previousRow = row;
}

void SchoolBillManager::SelectedCellChangedHeaderTable()
{
    int sumUp = 0;
    for(int i=0; i<8; i = i + 2)
    {
        sumUp += dataInTable(headerTable,0,i);
    }
    viewModelHeaderTable->setItem(0,8,new QStandardItem(QString::number(sumUp)));

    sumUp = 0;
    for(int i=1; i<8; i = i + 2)
    {
        sumUp += dataInTable(headerTable,0,i);
    }
    viewModelHeaderTable->setItem(0,9,new QStandardItem(QString::number(sumUp)));

    sumUp = dataInTable(headerTable,0,8) + dataInTable(headerTable,0,9);
    viewModelHeaderTable->setItem(0,10,new QStandardItem(QString::number(sumUp)));
}


void SchoolBillManager::SaveSchoolEvent()
{
    if(currentSchool.isEmpty())
    {
        QMessageBox::information(0,"Select School","Life is not Race",0,0);
        return;
    }

    if(!checkData())
        qDebug() << "You are stupid Smart";

    if(query->exec(db->getCreateSchoolBillTable(getCurrentTableName())))
    {
        int progress = 0;
        progressBar->setValue(progress);
        progressBar->setVisible(true);

        if(query->exec(db->getInsertSchoolBill(getCurrentTableName(), 0,
                                                           dataInTable(headerTable,0,0),
                                                           dataInTable(headerTable,0,1),
                                                           dataInTable(headerTable,0,2),
                                                           dataInTable(headerTable,0,3),
                                                           dataInTable(headerTable,0,4),
                                                           dataInTable(headerTable,0,5),
                                                           dataInTable(headerTable,0,6),
                                                           dataInTable(headerTable,0,7),
                                                           dataInTable(headerTable,0,8),
                                                           dataInTable(headerTable,0,9),
                                                           dataInTable(headerTable,0,10))))
        {

        }else
            qDebug() << "Could not insert initial total";

        for(int row=0; row < 15; row++)
        {
            if(query->exec(db->getInsertSchoolBill(getCurrentTableName(),
                                                   dataInTable(table,row,0),
                                                   dataInTable(table,row,1),
                                                   dataInTable(table,row,2),
                                                   dataInTable(table,row,3),
                                                   dataInTable(table,row,4),
                                                   dataInTable(table,row,5),
                                                   dataInTable(table,row,6),
                                                   dataInTable(table,row,7),
                                                   dataInTable(table,row,8),
                                                   dataInTable(table,row,9),
                                                   dataInTable(table,row,10),
                                                   dataInTable(table,row,11))))
            {

            }else
                qDebug() << "Failed to insert day";
            query->finish();
            progress += 7; // 100/15 ~ 7
            progressBar->setValue(progress);
        }
        progressBar->setVisible(false);
    }else
        qDebug() << "Failed to create school bill table";
}

QString SchoolBillManager::getCurrentTableName()
{
    return "SchoolBill_" + currentSchool.trimmed() + "_" + currentSTD + "_" + currentPeriod
            + "_" + currentMonth + "_" + currentYear;
}


