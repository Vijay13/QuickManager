#include "schoolbillmanager.h"

#include <QHeaderView>
#include <QMessageBox>
//#include <QSqlError>

SchoolBillManager::SchoolBillManager(QPushButton* editButton,QProgressBar *progressBar, QListView *list,
                                     QTableView *headerTable,
                                     QTableView *tableAttendence, QTableView *tableAttendenceTotal,
                                     QTableView *tableBeneficiaries, QTableView *tableBeneficiariesTotal,
                                     QLabel *taluka, QLabel *school):
    editButton(editButton), progressBar(progressBar), headerTable(headerTable),
    tableAttendence(tableAttendence), tableAttendenceTotal(tableAttendenceTotal),
    tableBeneficiaries(tableBeneficiaries),tableBeneficiariesTotal(tableBeneficiariesTotal),
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
    setUpTables();
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
        if(!selectedTaluka.contains("All Taluka",Qt::CaseInsensitive))
        {
            if(selectedTaluka != schools->getSchoolList()->at(i)->Taluka)
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

void SchoolBillManager::setUpTables()
{
    viewModelTableAttendence = new QStandardItemModel(15,12);
    viewModelTableAttendenceTotal = new QStandardItemModel(15,6);
    viewModelTableBeneficiaries = new QStandardItemModel(15,12);
    viewModelTableBeneficiariesTotal = new QStandardItemModel(15,6);

    sortModelTableAttendence = new QSortFilterProxyModel();
    sortModelTableAttendenceTotal = new QSortFilterProxyModel();
    sortModelTableBeneficiaries = new QSortFilterProxyModel();
    sortModelTableBeneficiariesTotal = new QSortFilterProxyModel();

    sortModelTableAttendence->setSourceModel(viewModelTableAttendence);
    sortModelTableAttendenceTotal->setSourceModel(viewModelTableAttendenceTotal);
    sortModelTableBeneficiaries->setSourceModel(viewModelTableBeneficiaries);
    sortModelTableBeneficiariesTotal->setSourceModel(viewModelTableBeneficiariesTotal);

    tableAttendence->setModel(sortModelTableAttendence);
    tableAttendenceTotal->setModel(sortModelTableAttendenceTotal);
    tableBeneficiaries->setModel(sortModelTableBeneficiaries);
    tableBeneficiariesTotal->setModel(sortModelTableBeneficiariesTotal);

    viewModelTableAttendence->setHorizontalHeaderLabels( QStringList() << "Date"
                                                         << "SC-B" << "SC-G"
                                                         << "ST-B" << "SC-G"
                                                         << "OBC-B" << "OBC-G"
                                                         << "Gen-B" << "Gen-G"
                                                         << "Total-B" << "Total-G"
                                                         << "Total");

    viewModelTableAttendenceTotal->setHorizontalHeaderLabels(QStringList() << "T-B"
                                                             << "T-G" << "Total"
                                                             << "D-B" << "D-G"
                                                             << "Diff");

    viewModelTableBeneficiaries->setHorizontalHeaderLabels( QStringList() << "Date"
                                                            << "SC-B" << "SC-G"
                                                            << "ST-B" << "SC-G"
                                                            << "OBC-B" << "OBC-G"
                                                            << "Gen-B" << "Gen-G"
                                                            << "Total-B" << "Total-G"
                                                            << "Total");


    viewModelTableBeneficiariesTotal->setHorizontalHeaderLabels(QStringList() << "T-B"
                                                                << "T-G" << "Total"
                                                                << "D-B" << "D-G"
                                                                << "Diff");
    setUpZeroinTable();
}

void SchoolBillManager::resetTables()
{
    progressBar->setValue(0);
    progressBar->setVisible(true);
    int rowCount = 0;

    for(int column=0; column<8; column++)
    {
        viewModelHeaderTable->setItem(0,column,new QStandardItem(""));
    }

    ++rowCount;
    progressBar->setValue(rowCount*6.25);

    for(int row=0; row < 15; row++)
    {
        for(int column=1; column<12 ; column++)
        {
            viewModelTableAttendence->setItem(row,column,new QStandardItem(""));
            viewModelTableBeneficiaries->setItem(row,column,new QStandardItem(""));
        }
        ++rowCount;
        progressBar->setValue(rowCount*6.25);
    }

    progressBar->setVisible(false);
    setUpZeroinTable();
}

void SchoolBillManager::setUpZeroinTable()
{
    previousRow = -1;
    previousColumn = -1;
    for(int row=0; row<15; row++)
    {
        for(int column=0; column<6; column++)
        {
            viewModelTableAttendenceTotal->setItem(row,column,new QStandardItem("0"));
            viewModelTableBeneficiariesTotal->setItem(row,column,new QStandardItem("0"));
        }

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
        ++start;
        viewModelTableAttendence->setItem(i,0,new QStandardItem(QString::number(start)));
        viewModelTableBeneficiaries->setItem(i,0,new QStandardItem(QString::number(start)));
    }
}

void SchoolBillManager::navigatedToSBM(QString taluka, QString rout, int std, int period,
                                       QString month, QString year)
{
    currentSTD = (std == 0) ? "1to5" : "6th8";
    currentPeriod = (period == 0) ? "1to15" : "16to31";

    currentMonth = month;
    currentYear = year;
    currentSchool = "";
    selectedTaluka = taluka;
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
        currentCenter = tempSchool->CenterNo;
        currentTaluka = tempSchool->Taluka;
        taluka->setText(currentTaluka);
        schoolName->setText(currentSchool);

        if(query->exec(db->getSchoolBillTable(getCurrentAttendenceTableName())))
        {
            editButton->setEnabled(true);
        }
        else
        {
            editButton->setEnabled(false);
        }

        if(!isTableReset)
        {
            resetTables();
            isTableReset = true;
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

void SchoolBillManager::SelectedCellChangedMainBillTable(QTableView* currentTable)
{
    if(currentTable == tableAttendence)
    {
        currentTableTotal = tableAttendenceTotal;
        currentView = viewModelTableAttendenceTotal;
    }
    else if(currentTable == tableBeneficiaries)
    {
        currentTableTotal = tableBeneficiariesTotal;
        currentView = viewModelTableBeneficiariesTotal;
    }
    else
        return;

    int sum=0,row = currentTable->currentIndex().row();

    if(previousRow != row && previousRow != -1)
    {
        // counting boys
        for(int i=1;i<9;i+=2)
        {
            sum += dataInTable(currentTable,previousRow,i);
        }
        currentView->setItem(previousRow,0,new QStandardItem(QString::number(sum)));

        //counting girls
        sum = 0;
        for(int i=2;i<9;i+=2)
        {
            sum += dataInTable(currentTable,previousRow,i);
        }
        currentView->setItem(previousRow,1,new QStandardItem(QString::number(sum)));

        //counting total strength
        sum = dataInTable(currentTableTotal,previousRow,0) + dataInTable(currentTableTotal,previousRow,1);
        currentView->setItem(previousRow,2,new QStandardItem(QString::number(sum)));

        //seting up diff columns
        sum = dataInTable(currentTableTotal,previousRow,0) - dataInTable(currentTable,previousRow,9);
        currentView->setItem(previousRow,3,new QStandardItem(QString::number(sum)));

        sum = dataInTable(currentTableTotal,previousRow,1) - dataInTable(currentTable,previousRow,10);
        currentView->setItem(previousRow,4,new QStandardItem(QString::number(sum)));

        sum = dataInTable(currentTableTotal,previousRow,2) - dataInTable(currentTable,previousRow,11);
        currentView->setItem(previousRow,5,new QStandardItem(QString::number(sum)));
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

void SchoolBillManager::EditSchoolEvent()
{
    if(query->exec(db->getSchoolBillTable(getCurrentAttendenceTableName())))
    {
        progressBar->setValue(0);
        progressBar->setVisible(true);
        int row = 0;
        query->next();
        for(int column=0; column<11; column++)
        {
            viewModelHeaderTable->setItem(0,column,new QStandardItem(dataForTable(query->value(column).toInt())));
        }

        while(query->next())
        {
            for(int column=0; column<12; column++)
            {
                viewModelTableAttendence->setItem(row,column,new QStandardItem(dataForTable(query->value(column).toInt())));
            }
            for(int column=12; column<18; column++ )
            {
                viewModelTableAttendenceTotal->setItem(row,column - 12,new QStandardItem(query->value(column).toString()));
            }
            row++;
            progressBar->setValue(row*7);
        }
        progressBar->setVisible(false);
        isTableReset = false;
    }

    query->finish();
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

    if(query->exec(db->getCreateSchoolBillTable(getCurrentAttendenceTableName())))
    {
        int progress = 0;
        progressBar->setValue(progress);
        progressBar->setVisible(true);

        if(query->exec(db->getInsertSchoolBill(getCurrentAttendenceTableName(), 0,
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
                                               dataInTable(headerTable,0,10),
                                               0,0,0,0,0,0)))
        {

        }else
            qDebug() << "Could not insert initial total";

        for(int row=0; row < 15; row++)
        {
            if(query->exec(db->getInsertSchoolBill(getCurrentAttendenceTableName(),
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
                                                   dataInTable(tableAttendence,row,11),
                                                   dataInTable(tableAttendenceTotal,row,0),
                                                   dataInTable(tableAttendenceTotal,row,1),
                                                   dataInTable(tableAttendenceTotal,row,2),
                                                   dataInTable(tableAttendenceTotal,row,3),
                                                   dataInTable(tableAttendenceTotal,row,4),
                                                   dataInTable(tableAttendenceTotal,row,5))))
            {
                editButton->setEnabled(true);
            }else
                qDebug() << "Failed to insert day";
            query->finish();
            progress += 7; // 100/15 ~ 7
            progressBar->setValue(progress);
        }
        progressBar->setVisible(false);
        isTableReset = false;
    }else
        qDebug() << "Failed to create school bill table";
}

void SchoolBillManager::DeleteSchoolEvent()
{
    if(currentSchool.isEmpty())
    {
        QMessageBox::information(0,"Select School","Please select school",0,0);
        return;
    }

    if(query->exec(db->getDeleteSchoolBill(getCurrentAttendenceTableName())))
    {
        resetTables();
    }
    else
        qDebug() << db->getDeleteSchoolBill(getCurrentAttendenceTableName())
                 << "Could not delete School Bill" <<  query->lastError().text()
                 << query->isActive();
}

QString SchoolBillManager::getCurrentAttendenceTableName()
{
    return "SchoolBill_Attendence_" + currentCenter.remove(' ') + "_"
            + currentTaluka.remove(' ') + "_" + currentSTD + "_" + currentPeriod + "_"
            + currentMonth + "_" + currentYear;
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
