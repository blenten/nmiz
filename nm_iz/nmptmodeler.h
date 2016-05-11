#ifndef NMPTMODELER_H
#define NMPTMODELER_H

#include <vector>
#include <tuple>
#include <cmath>

using namespace std;

struct StonePoint
{
    double x, y, Vx, Vy;
};

struct DuckPoint
{
    double x, Vx;
};


class NMPTmodeler
{
public:
    NMPTmodeler(double V, double U, double tau, double g, double dX0, double k_g, double k_sv, double m_st, double alpha);
    void buildModel(int iterations_number);
    vector<StonePoint> getStoneTrajectory();
    vector<DuckPoint> getDuckTrajectory();
    pair<double, int> getClosestEncounter();
private:
    void moveDuck(int iteration);
    void moveStone(int iteration);

    vector<StonePoint> stone_trajectory;
    vector<DuckPoint> duck_trajectory;
    double V, U, tau, g, dX0, k_g, k_sv, m_st, alpha;
    pair<double, int> closest_encounter;
};

#endif // NMPTMODELER_H
