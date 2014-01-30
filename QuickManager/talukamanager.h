#ifndef TALUKAMANAGER_H
#define TALUKAMANAGER_H

#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "DataManager/filemanager.h"
#include "DataModel/alltaluka.h"

namespace Ui {
class TalukaManager;
}

class TalukaManager : public QDialog
{
    Q_OBJECT

public:
    explicit TalukaManager(QWidget *parent = 0);
    ~TalukaManager();

    void initialize();

    void setUpTable();

    void setUpControl(int TMAdd, int TMUpdate, int TMDelete);

private slots:
    void addTaluka();

    void editTaluka();

    void removeTaluka();

    void updateTableView();

    void printTaluka();

    void exportTaluka();

private:
    Ui::TalukaManager *ui;
    QTableView* table;
    AllTaluka* talukas;
    FileManager* fm;
    QStandardItemModel* viewModel;
    QSortFilterProxyModel* sortModel;
};

#endif // TALUKAMANAGER_H
