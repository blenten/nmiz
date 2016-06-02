#include "nmpt_simulator.h"

string StonePoint::ToString()
{
    string res = std::to_string(x)+'\t'+std::to_string(y)+'\t'+std::to_string(Vx)+'\t'+std::to_string(Vy)+'\n';
    return res;
}

string DuckPoint::ToString()
{
    string res = std::to_string(x)+'\t'+std::to_string(Vx)+'\n';
    return res;
}

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

QVector<StonePoint> NMPT_simulator::getStoneTrajectory()
{

    return stone_trajectory;
}

QVector<DuckPoint> NMPT_simulator::getDuckTrajectory()
{
    return duck_trajectory;
}

void NMPT_simulator::resize_trajectory(int max_iter)
{
    QVector<StonePoint> temp_st;
    QVector<DuckPoint> temp_dck;
    temp_st.resize(max_iter);
    temp_dck.resize(max_iter);

    for(int i=0; i<max_iter;i++)
    {
        temp_dck[i]=duck_trajectory[i];
        temp_st[i]=stone_trajectory[i];
    }

    stone_trajectory.clear();
    duck_trajectory.clear();

    stone_trajectory = temp_st;
    duck_trajectory = temp_dck;
}

void NMPT_simulator::set_data(double V, double U, double tau, double g, double dX0,
                              double k_g, double k_sv, double m_st, double alpha)
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
            resize_trajectory(i);
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


NMPT_simulator_modified::NMPT_simulator_modified(double V, double U, double tau, double g, double dX0,
                                                 double k_g, double k_sv, double m_st, double alpha)
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


void NMPT_simulator_modified::moveDuck(int iteration)
{
    //X coord
    duck_trajectory[iteration].x = duck_trajectory[iteration-1].Vx * tau + duck_trajectory[iteration-1].x;
    //X vel
    duck_trajectory[iteration].Vx = U;
}

void NMPT_simulator_modified::moveStone(int iteration)
{
    //X ve
    stone_trajectory[iteration].Vx = stone_trajectory[iteration-1].Vx*(1 - (tau/2)*(k_sv/m_st))/(1 + (tau/2)*(k_sv/m_st));
    //Y vel
    stone_trajectory[iteration].Vy = (stone_trajectory[iteration-1].Vy*(1 + pow(tau,2)*k_g/4 - (tau/2)*(k_sv/m_st)) - tau*(g - k_g*stone_trajectory[iteration-1].y))
                                    / (1 - pow(tau,2)*k_g/4 + (tau/2)*(k_sv/m_st));

    //X coord
    stone_trajectory[iteration].x = stone_trajectory[iteration-1].x + tau/2*(stone_trajectory[iteration-1].Vx+stone_trajectory[iteration].Vx);
    //Y coord
    stone_trajectory[iteration].y = stone_trajectory[iteration-1].y + tau/2*(stone_trajectory[iteration-1].Vy+stone_trajectory[iteration].Vy);
}
