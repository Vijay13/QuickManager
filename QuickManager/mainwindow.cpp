#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maindatabase.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainDatabase *db = new MainDatabase();
    db->getDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}
