#pragma once
#include <QMainWindow>
#include <QGroupBox>
#include <QVBoxLayout>
#include "customGraphicsView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    Ui::MainWindow *ui;
    QGroupBox* group;
    QVBoxLayout* vLayout;
    customGraphicsView* view;
public slots:
    void on_actionBuilding_Polygon_Mode_triggered();
    void on_actionExit_triggered();
    void on_actionClear_All_triggered();
    void on_statusChanged(QString& message);

signals:
    void polygonModeChanged();
    void clearAll();
};
