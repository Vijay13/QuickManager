#include "filemanager.h"

#include <QDebug>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrintDialog>

FileManager::FileManager():table(NULL)
{
    printer = new QPrinter();
    instance = this;
}

FileManager::FileManager(QTableView* table): table(table)
{
    printer = new QPrinter();
    instance = this;
}

FileManager* FileManager::instance = 0;

FileManager* FileManager::Instance(){
    return instance;
}

void FileManager::exportCSV(){

    if(table == NULL)
        return;

    QString filePath = QFileDialog::getSaveFileName(0,"Save","",".csv",0,0);

    if(!filePath.contains(".csv"))
        filePath.append(".csv");

    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);

        //writing column headers in file
        for (int column = 0; column < table->model()->columnCount(); column++)
            if (!table->isColumnHidden(column))
                out << QString("%1,").arg(table->model()->headerData(column, Qt::Horizontal).toString());

        //writing raws in file
        out << "\n";
        for(int row=0; row < table->model()->rowCount(); row++){
            for(int column=0; column < table->model()->columnCount(); column++){
                out << table->model()->data(table->model()->index(row,column)).toString() + ",";
            }
            out << "\n";
        }
    }
    file.close();
}

void FileManager::printTable(){

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrintDialog *dialog = new QPrintDialog(printer, NULL);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(printer);
    }
}

void FileManager::writeTable(QTableView* table)
{
    if(table == NULL)
        return;

    QTextStream out(&strStream);
    const int rowCount = table->model()->rowCount();
    const int columnCount = table->model()->columnCount();

    out <<  "<html>\n"
            "<head>\n"
            "<meta Content=\"Text/html; charset=Windows-1251\">\n"
         <<  QString("<title>%1</title>\n").arg(table->accessibleName())
          <<  "</head>\n"
              "<body bgcolor=#ffffff link=#5000A0>\n"
              "<table border=1 cellspacing=0 cellpadding=2>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        if (!table->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(table->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!table->isColumnHidden(column)) {
                QString data = table->model()->data(table->model()->index(row, column)).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out <<  "</table>\n"
            "</body>\n"
            "</html>\n" << "<br /><br />";
}

void FileManager::clearDocument()
{
    this->strStream = "";
}
