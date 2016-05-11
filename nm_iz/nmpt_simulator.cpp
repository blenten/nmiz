#include "nmpt_simulator.h"

NMPT_simulator::NMPT_simulator(double V, double U, double tau, double g, double dX0, double k_g, double k_sv, double m_st, double alpha)
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

    max_iter_achieved = -1;
}

QVector<double> NMPT_simulator::getDuckX()
{
    QVector<double> duck_x;
    duck_x.resize(max_iter_achieved);

    for(int i=0; i<max_iter_achieved; i++)
    {
        duck_x[i] = duck_trajectory[i].x;
    }
    return duck_x;
}

QVector<double> NMPT_simulator::getStoneX()
{
    QVector<double> stone_x;
    stone_x.resize(max_iter_achieved);

    for(int i=0; i<max_iter_achieved; i++)
    {
        stone_x[i] = stone_trajectory[i].x;
    }
    return stone_x;
}

QVector<double> NMPT_simulator::getStoneY()
{
    QVector<double> stone_y;
    stone_y.resize(max_iter_achieved);

    for(int i=0; i<max_iter_achieved; i++)
    {
        stone_y[i] = stone_trajectory[i].y;
    }
    return stone_y;
}

void NMPT_simulator::buildModel(int iterations_number)
{
    stone_trajectory.resize(iterations_number);
    duck_trajectory.resize(iterations_number);

    max_iter_achieved = iterations_number;

    stone_trajectory[0].x = 0;
    stone_trajectory[0].y=0;
    stone_trajectory[0].Vx = qCos(qDegreesToRadians(alpha))*V;
    stone_trajectory[0].Vy = qSin(qDegreesToRadians(alpha))*V;

    duck_trajectory[0].x = dX0;
    duck_trajectory[0].Vx = U;

    closest_encounter.first = dX0;
    closest_encounter.second = 0;

    for(int i=1; i<iterations_number; i++)
    {
        moveDuck(i);
        moveStone(i);

        if(stone_trajectory[i].y < 0)
        {
            max_iter_achieved = i;
            break;
        }

        if(abs(stone_trajectory[i].x - duck_trajectory[i].x) < closest_encounter.first)
        {
            closest_encounter.first = abs(stone_trajectory[i].x - duck_trajectory[i].x);
            closest_encounter.second = i;
        }
    }
}

void NMPT_simulator::moveDuck(int iteration)
{
    //X coord
    duck_trajectory[iteration].x = duck_trajectory[iteration-1].Vx * tau + duck_trajectory[iteration-1].x;
    //X vel
    duck_trajectory[iteration].Vx = U;
}
void NMPT_simulator::moveStone(int iteration)
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

pair<double, int> NMPT_simulator::getClosestEncounter()
{
    return closest_encounter;
}
