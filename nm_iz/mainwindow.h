#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "nmpt_simulator.h"
#include <QVector>
#include <iostream>
#include <fstream>

enum SimulatorType
{
    NMPT_simple,
    NMPT_modified
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_build_model_button_clicked();

    void on_saveButton_clicked();

    void on_simType_box_toggled(bool checked);

private:

    void buildStoneChart(QVector<double> X, QVector<double> Y);
    void buildDuckChart(QVector<double> X, QVector<double> Y, int closest_index);

    NMPT_simulator* createSimulator();
    SimulatorType simType;

    Ui::MainWindow *ui;
    NMPT_simulator *nmpt;
};

#endif // MAINWINDOW_H
