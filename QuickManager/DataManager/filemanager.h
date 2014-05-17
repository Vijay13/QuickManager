#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QTableView>
#include <QTextStream>
#include <QPrinter>


/*
 * This class implements functionalities like print and export
 * for use from anywhere in Application
 * */
class FileManager
{
public:
    FileManager();

    FileManager(QTableView* table);

    static FileManager* Instance();

    /*
     * Exports last set table into .csv file
     * */
    void exportCSV();

    /*
     * Prints last set table along with headers and lines
     * */
    void printTable();

    void writeHeader(QString title);

    void writeLine(QString line);

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
