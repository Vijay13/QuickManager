#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QTableView>
#include <QTextStream>
#include <QPrinter>

class FileManager
{
public:
    FileManager();

    FileManager(QTableView* table);

    static FileManager* Instance();

    void exportCSV();

    void printTable();

    void writeTable(QTableView* table);

    void clearDocument();

    void setTable(QTableView* table){ this->table = table;}

    QTableView* getTable(){ return getTable();}

private:
    static FileManager* instance;
    QTableView* table;
    QPrinter* printer;
    QString strStream;
};

#endif // FILEMANAGER_H
