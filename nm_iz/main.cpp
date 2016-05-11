#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    NMPT_simulator nmpt(14, 5, 0.01, 9.8, 4, 0.05, 0.003, 0.5, 25);
    nmpt.buildModel(1001);
    double cl;
    int cl_index;

    std::tie(cl, cl_index) = nmpt.getClosestEncounter();
    return a.exec();
}
