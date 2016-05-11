#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "nmpt_simulator.h"
#include <QVector>

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

private:

    void buildStoneChart(QVector<double> X, QVector<double> Y);
    void buildDuckChart(QVector<double> X, QVector<double> Y, int closest_index);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
