#ifndef TALUKAMANAGER_H
#define TALUKAMANAGER_H

#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <alltaluka.h>

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

private slots:
    void addTaluka();

    void editTaluka();

    void removeTaluka();

    void updateTableView();

private:
    Ui::TalukaManager *ui;
    QTableView* table;
    AllTaluka* talukas;
    QStandardItemModel* viewModel;
    QSortFilterProxyModel* sortModel;
};

#endif // TALUKAMANAGER_H
