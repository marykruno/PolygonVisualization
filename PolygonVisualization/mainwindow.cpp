#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new customGraphicsView();
    group = new QGroupBox();
    vLayout =new QVBoxLayout();
    vLayout->addWidget(view);
    group->setLayout(vLayout);
    setCentralWidget(group);
    connect(this, SIGNAL(polygonModeChanged()), view, SLOT(on_optionsPolygonModeChanged()));
    connect(this, SIGNAL(clearAll()), view, SLOT(on_optionsClearAll()));
    connect(view, SIGNAL(updateStatus(QString&)), this, SLOT(on_statusChanged(QString&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionBuilding_Polygon_Mode_triggered()
{
    emit polygonModeChanged();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionClear_All_triggered()
{
    emit clearAll();
}

void MainWindow::on_statusChanged(QString& message){
    ui->statusbar->showMessage(message);
}
