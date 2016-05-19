#ifndef NMPTMODELER_H
#define NMPTMODELER_H

#include <tuple>
#include <QtMath>
#include <string>
#include <QVector>

using namespace std;

class StonePoint
{
public:
    string ToString();
    double x, y, Vx, Vy;
};

class DuckPoint
{
public:
    string ToString();
    double x, Vx;
};


class NMPT_simulator
{
public:
    NMPT_simulator(double V, double U, double tau, double g, double dX0, double k_g, double k_sv, double m_st, double alpha);

    void buildModel(int iterations_number);

    QVector<double> getStoneX();
    QVector<double> getStoneY();
    QVector<double> getDuckX();
    QVector<DuckPoint> getDuckTrajectory();
    QVector<StonePoint> getStoneTrajectory();

    pair<double, int> getClosestEncounter();

private:
    void moveDuck(int iteration);
    void moveStone(int iteration);

    void resize_trajectory(int max_iter);

    QVector<StonePoint> stone_trajectory;
    QVector<DuckPoint> duck_trajectory;

    int max_iter_achieved;

    double V, U, tau, g, dX0, k_g, k_sv, m_st, alpha;

    pair<double, int> closest_encounter;
};

#endif // NMPTMODELER_H
