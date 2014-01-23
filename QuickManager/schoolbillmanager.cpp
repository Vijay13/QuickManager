#include "schoolbillmanager.h"
#include <QHeaderView>
#include <QMessageBox>

SchoolBillManager::SchoolBillManager(QProgressBar* progressBar,QListView* list,
                                     QTableView* headerTable, QTableView* tableAttendence,
                                     QLabel* taluka, QLabel* school):
    progressBar(progressBar), headerTable(headerTable),tableAttendence(tableAttendence),
    list(list), taluka(taluka), schoolName(school), isTableReset(true)
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

void SchoolBillManager::setUpList()
{
    viewModelList->clear();
    for(int i=0 ; i< schools->getSchoolList()->length() ; i++)
    {
        if(!currentTaluka.contains("All Taluka",Qt::CaseInsensitive))
        {
            if(currentTaluka != schools->getSchoolList()->at(i)->Taluka)
                continue;
        }

        if(!currentRout.contains("All Rout",Qt::CaseInsensitive))
        {
            if(currentRout != schools->getSchoolList()->at(i)->RoutNo)
                continue;
        }

        viewModelList->appendRow(new QStandardItem(schools->getSchoolList()->at(i)->SchoolName));
    }
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
}

void SchoolBillManager::setUpTable()
{
    viewModelTableAttendence = new QStandardItemModel(15,12);
    sortModelTableAttendence = new QSortFilterProxyModel();
    sortModelTableAttendence->setSourceModel(viewModelTableAttendence);
    tableAttendence->setModel(sortModelTableAttendence);
    viewModelTableAttendence->setHorizontalHeaderLabels( QStringList() << "Date"
                                               << "SC-B" << "SC-G"
                                               << "ST-B" << "SC-G"
                                               << "OBC-B" << "OBC-G"
                                               << "Gen-B" << "Gen-G"
                                               << "Total-B" << "Total-G"
                                               << "Total");
    setUpZeroinTable();
}

void SchoolBillManager::resetTables()
{
    for(int column=0; column<8; column++)
    {
        viewModelHeaderTable->setItem(0,column,new QStandardItem(""));
    }

    for(int row=0; row < 15; row++)
    {
        for(int column=1; column<9 ; column++)
        {
            viewModelTableAttendence->setItem(row,column,new QStandardItem(""));
        }
    }
    setUpZeroinTable();
}

void SchoolBillManager::setUpZeroinTable()
{
    previousRow = -1;
    previousColumn = -1;
    for(int row=0; row<15; row++)
    {
        for(int column=9; column<12; column++)
            viewModelTableAttendence->setItem(row,column,new QStandardItem("0"));
    }

    viewModelHeaderTable->setItem(0,8,new QStandardItem("0"));
    viewModelHeaderTable->setItem(0,9,new QStandardItem("0"));
    viewModelHeaderTable->setItem(0,10,new QStandardItem("0"));
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
        viewModelTableAttendence->setItem(i,0,new QStandardItem(QString::number(++start)));
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
    currentTaluka = taluka;
    currentRout = rout;

    setDates();
    setUpList();
}

void SchoolBillManager::schoolChanged()
{
    School* tempSchool = schools->getSchool(list->currentIndex().data(Qt::DisplayRole).toString());

    if(tempSchool != 0)
    {
        currentSchool = tempSchool->SchoolName;
        taluka->setText(tempSchool->Taluka);
        schoolName->setText(currentSchool);
        if(query->exec(db->getSchoolBillTable(getCurrentTableName())))
        {
            progressBar->setValue(0);
            progressBar->setVisible(true);
            int row = 0;
            query->next();
            for(int column=0; column<11; column++)
            {
                viewModelHeaderTable->setItem(0,column,new QStandardItem(dataForTable(query->value(column).toInt())));
            }

            while (query->next()) {
                for(int column=0; column<12; column++)
                {
                    viewModelTableAttendence->setItem(row,column,new QStandardItem(dataForTable(query->value(column).toInt())));
                }
                row++;
                progressBar->setValue(row*7);
            }
            progressBar->setVisible(false);
            isTableReset = false;
        }
        else
        {
            if(!isTableReset)
                resetTables();
        }
        query->finish();
    }
}

bool SchoolBillManager::checkData()
{
    int row,column;

    for(int i=0; i<15; i++)
    {
        if(dataInTable(tableAttendence,i,0) < 1 || dataInTable(tableAttendence,i,0) > 31)
        {
            QMessageBox::information(0,"Invalid Date","Date can not be " + QString::number(dataInTable(tableAttendence,i,0))
                                     + " at " + QString::number(i) ,0,0);
            return false;
        }
    }
    for(row=0; row < 15; row++)
    {
        for(column=1; column < 12; column ++)
        {
            if(!(dataInTable(tableAttendence,row,column) <= dataInTable(headerTable,0,column-1)))
            {
                qDebug() << "Not fair at: " << row << column;
            }
        }
    }

    return true;
}

void SchoolBillManager::SchoolCenterNameChanged(QString centerNo)
{
    if(centerNo.isEmpty())
    {
        setUpList();
        return;
    }

    viewModelList->clear();

    for(int i=0 ; i< schools->getSchoolList()->length() ; i++)
    {
        if(!schools->getSchoolList()->at(i)->CenterNo.contains(centerNo,Qt::CaseInsensitive))
            continue;

        viewModelList->appendRow(new QStandardItem(schools->getSchoolList()->at(i)->SchoolName));
    }
}

void SchoolBillManager::SelectedCellChangedBillTable()
{
    int sum=0,row = tableAttendence->currentIndex().row();

    if(previousRow != row && previousRow != -1)
    {
        // counting boys
        for(int i=1;i<9;i+=2)
        {
            sum += dataInTable(tableAttendence,previousRow,i);
        }
        viewModelTableAttendence->setItem(previousRow,9,new QStandardItem(QString::number(sum)));

        //counting girls
        sum = 0;
        for(int i=2;i<9;i+=2)
        {
            sum += dataInTable(tableAttendence,previousRow,i);
        }
        viewModelTableAttendence->setItem(previousRow,10,new QStandardItem(QString::number(sum)));

        //counting total strength
        sum = dataInTable(tableAttendence,previousRow,9) + dataInTable(tableAttendence,previousRow,10);
        viewModelTableAttendence->setItem(previousRow,11,new QStandardItem(QString::number(sum)));
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
        QMessageBox::information(0,"Select School","Please select school",0,0);
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
                                                   dataInTable(tableAttendence,row,0),
                                                   dataInTable(tableAttendence,row,1),
                                                   dataInTable(tableAttendence,row,2),
                                                   dataInTable(tableAttendence,row,3),
                                                   dataInTable(tableAttendence,row,4),
                                                   dataInTable(tableAttendence,row,5),
                                                   dataInTable(tableAttendence,row,6),
                                                   dataInTable(tableAttendence,row,7),
                                                   dataInTable(tableAttendence,row,8),
                                                   dataInTable(tableAttendence,row,9),
                                                   dataInTable(tableAttendence,row,10),
                                                   dataInTable(tableAttendence,row,11))))
            {

            }else
                qDebug() << "Failed to insert day";
            query->finish();
            progress += 7; // 100/15 ~ 7
            progressBar->setValue(progress);
        }
        progressBar->setVisible(false);
    }else
        qDebug() << db->getCreateSchoolBillTable(getCurrentTableName())
                 << "Failed to create school bill table";
}

void SchoolBillManager::DeleteSchoolEvent()
{
    if(currentSchool.isEmpty())
    {
        QMessageBox::information(0,"Select School","Please select school",0,0);
        return;
    }

    if(query->exec(db->getDeleteSchoolBill(getCurrentTableName())))
    {
        resetTables();
    }
    else
        qDebug() << db->getDeleteSchoolBill(getCurrentTableName())
                 << "Could not delete School Bill" <<  query->lastError().text()
                 << query->isActive();
}

QString SchoolBillManager::getCurrentTableName()
{
    return "SchoolBill_" + currentSchool.remove(' ') + "_" + currentSTD + "_" + currentPeriod
            + "_" + currentMonth + "_" + currentYear;
}

QString SchoolBillManager::dataForTable(int data)
{
    if(data != 0)
        return QString::number(data);
    return "";
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
