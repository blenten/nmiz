#include "nmptmodeler.h"

NMPTmodeler::NMPTmodeler(double V, double U, double tau, double g, double dX0, double k_g, double k_sv, double m_st, double alpha)
{
    this->tau = tau;
    this->g = g;
    this->dX0 = dX0;
    this->k_g = k_g;
    this->k_sv = k_sv;
    this->m_st = m_st;
    this->alpha = alpha;
    this->U = U;
    this->V = V;
}

vector<StonePoint> NMPTmodeler::getStoneTrajectory()
{
    return stone_trajectory;
}
vector<DuckPoint> NMPTmodeler::getDuckTrajectory()
{
    return duck_trajectory;
}

void NMPTmodeler::buildModel(int iterations_number)
{
    stone_trajectory.resize(iterations_number);
    duck_trajectory.resize(iterations_number);

    stone_trajectory[0].x = 0;
    stone_trajectory[0].y=0;
    stone_trajectory[0].Vx = cos(alpha)*V;
    stone_trajectory[0].Vy = sin(alpha)*V;

    duck_trajectory[0].x = dX0;
    duck_trajectory[0].Vx = U;

    closest_encounter.first = dX0;
    closest_encounter.second = 0;

    for(int i=1; i<iterations_number; i++)
    {
        moveDuck(i);
        moveStone(i);

        if(stone_trajectory[i].y < 0) break;

        if(abs(stone_trajectory[i].x - duck_trajectory[i].x) < closest_encounter.first)
        {
            closest_encounter.first = abs(stone_trajectory[i].x - duck_trajectory[i].x);
            closest_encounter.second = i;
        }
    }
}

void NMPTmodeler::moveDuck(int iteration)
{
    //X coord
    duck_trajectory[iteration].x = duck_trajectory[iteration-1].Vx * tau + duck_trajectory[iteration-1].x;
    //X vel
    duck_trajectory[iteration].Vx = U;
}
void NMPTmodeler::moveStone(int iteration)
{
    //X coord
    stone_trajectory[iteration].x = stone_trajectory[iteration-1].Vx * tau + stone_trajectory[iteration-1].x;
    //Y coord
    stone_trajectory[iteration].y = stone_trajectory[iteration-1].Vy * tau + stone_trajectory[iteration-1].y;

    //X vel
    double a_x = -(k_sv * stone_trajectory[iteration-1].Vx / m_st);
    stone_trajectory[iteration].Vx = a_x * tau + stone_trajectory[iteration-1].Vx;
    //Y vel
    double a_y = -g + (k_g * stone_trajectory[iteration-1].y) - (k_sv * stone_trajectory[iteration-1].Vy / m_st);
    stone_trajectory[iteration].Vy = a_y * tau + stone_trajectory[iteration-1].Vy;
}

pair<double, int> NMPTmodeler::getClosestEncounter()
{
    return closest_encounter;
}
