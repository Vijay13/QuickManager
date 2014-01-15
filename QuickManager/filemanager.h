#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QTableView>
#include <QPrinter>

class FileManager
{
public:
    FileManager();

    FileManager(QTableView* table);

    static FileManager* Instance();

    void exportCSV();

    void printTable();

    void setTable(QTableView* table){ this->table = table;}

    QTableView* getTable(){ return getTable();}

private:
    static FileManager* instance;
    QTableView* table;
    QPrinter* printer;
};

#endif // FILEMANAGER_H
