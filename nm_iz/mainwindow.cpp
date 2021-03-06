#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nmpt = nullptr;
    simType = NMPT_simple;
}

MainWindow::~MainWindow()
{
    delete ui;

    if(nmpt!=nullptr) delete nmpt;
}

NMPT_simulator* MainWindow::createSimulator()
{
    if(simType==NMPT_modified)
    {
        return new NMPT_simulator_modified();
    }
     return new NMPT_simulator();
}

void MainWindow::on_build_model_button_clicked()
{
    double U, V, alpha, k_g, k_sv, m, g, T, dX0;
    U = ui->U_line->text().toDouble();
    V = ui->V_line->text().toDouble();
    alpha = ui->alpha_line->text().toDouble();
    k_g = ui->k_g_line->text().toDouble();
    k_sv = ui->k_sv_line->text().toDouble();
    m = ui->m_line->text().toDouble();
    g = ui->g_line->text().toDouble();
    T = ui->tau_line->text().toDouble();
    dX0 = ui->dX0_line->text().toDouble();

    if(nmpt!=nullptr) delete nmpt;
    nmpt = createSimulator();
    nmpt->set_data(V,U,T,g,dX0,k_g,k_sv,m,alpha);
    nmpt->buildModel(ui->iter_line->text().toDouble());

    ui->chart_widget->clearGraphs();
    buildStoneChart(nmpt->getStoneX(),nmpt->getStoneY());
    double closest;
    int closest_index;
    std::tie(closest, closest_index) = nmpt->getClosestEncounter();
    if(closest < 1)
    {
        buildDuckChart(nmpt->getDuckX(), nmpt->getStoneY(), closest_index);
    }
}

void MainWindow::buildDuckChart(QVector<double> X, QVector<double> Y, int closest_index)
{
    ui->chart_widget->addGraph();
    QVector<double> Y_vec;
    Y_vec.resize(X.size());
    Y_vec.fill(Y[closest_index]);
    ui->chart_widget->graph(1)->setData(X, Y_vec);
    ui->chart_widget->graph(1)->setPen(QColor(255,0,0,255));

    ui->chart_widget->replot();
}

void MainWindow::buildStoneChart(QVector<double> X, QVector<double> Y)
{
    ui->chart_widget->addGraph();
    ui->chart_widget->graph(0)->setData(X, Y);

    ui->chart_widget->xAxis->setLabel("X");
    ui->chart_widget->yAxis->setLabel("Y");

    ui->chart_widget->xAxis->setRange(0, X[X.size()-1]);

    double  Ymax = 5;
    for(int i=1; i<(int)Y.size();i++)
    {
        if(Ymax<Y[i])   Ymax = Y[i];
    }

    ui->chart_widget->yAxis->setRange(0, Ymax);

    ui->chart_widget->replot();
}

void MainWindow::on_saveButton_clicked()
{
    QVector<StonePoint> st_traj = nmpt->getStoneTrajectory();
    QVector<DuckPoint> dck_traj = nmpt->getDuckTrajectory();

    ofstream fout;
    fout.open("output.txt");

    fout<<"STONE\n(X, Y, Vx, Vy)\n";
    for(StonePoint sp : st_traj)
    {
        fout << sp.ToString();
    }
    fout<<"\n\nDUCK\n(X, Vx)\n";
    for(DuckPoint dp : dck_traj)
    {
        fout << dp.ToString();
    }

    fout.close();
}

void MainWindow::on_simType_box_toggled(bool checked)
{
    if(checked)
    {
        simType = NMPT_modified;
    }else
    {
        simType = NMPT_simple;
    }
}
