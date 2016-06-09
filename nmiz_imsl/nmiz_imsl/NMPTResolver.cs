using System;
using System.Linq;
using Imsl.Math;

namespace nmiz_imsl
{
    public static class ResolverInputData
    {
        public static double x0 = 0.0, y0 = 0.0, dx0 = 4.0;
        public static double U = 5.0, V = 14.0, alpha = 25.0;
        public static double g = 9.8, kg = 0.05, ksv = 0.003, m = 0.5;
    }

    public class NMPTREsolverException : ApplicationException
    {
        public NMPTREsolverException() { }
        public NMPTREsolverException(string message) : base(message) { }
    }
    
    class NMPTResolver
    {
        private class NMPT_StoneFunc : OdeRungeKutta.IFunction
        {
            public void F(double x, double[] y, double[] yprime)
            {
                //Vx
                yprime[0] = -(ResolverInputData.ksv * y[0]) / ResolverInputData.m;
                //Vy
                yprime[1] = -ResolverInputData.g + ResolverInputData.kg * y[3] - (ResolverInputData.ksv * y[1]) / ResolverInputData.m;
                //X
                yprime[2] = y[0];
                //Y
                yprime[3] = y[1];
            }
        }
        private class StoneTrajectory
        {
            public double[] X, Y, Vx, Vy;

            public StoneTrajectory(int len)
            {
                X = new double[len];
                Y = new double[len];
                Vx = new double[len];
                Vy = new double[len];
            }
        }
        private class DuckTrajectory
        {
            public double[] X;
            public double Y;

            public DuckTrajectory(int len)
            {
                X = new double[len];
                Y = -1.0;
            }
        }


        public double[] StoneX
        {
            get
            {
                if(stoneTraj.X.Any())
                {
                    return stoneTraj.X;
                }
                else
                {
                    throw new NMPTREsolverException("No Trajectory!");
                }
            }
        }
        public double[] StoneY
        {
            get
            {
                if (stoneTraj.Y.Any())
                {
                    return stoneTraj.Y;
                }
                else
                {
                    throw new NMPTREsolverException("No Trajectory!");
                }
            }
        }
        public double[] StoneVx
        {
            get
            {
                if (stoneTraj.Vx.Any())
                {
                    return stoneTraj.Vx;
                }
                else
                {
                    throw new NMPTREsolverException("No Trajectory!");
                }
            }
        }
        public double[] StoneVy
        {
            get
            {
                if (stoneTraj.Vy.Any())
                {
                    return stoneTraj.Vy;
                }
                else
                {
                    throw new NMPTREsolverException("No Trajectory!");
                }
            }
        }
        public double[] DuckX
        {
            get
            {
                if (duckTraj.X.Any())
                {
                    return duckTraj.X;
                }
                else
                {
                    throw new NMPTREsolverException("No Trajectory!");
                }
            }
        }
        public double PossibleDuckY
        {
            get { return duckTraj.Y; }
        }

        StoneTrajectory stoneTraj;
        DuckTrajectory duckTraj;

        private void resizeTraj(int len)
        {
            StoneTrajectory temps = new StoneTrajectory(len);
            DuckTrajectory tempg = new DuckTrajectory(len);
            for(int i=0; i<len; i++)
            {
                temps.X[i] = stoneTraj.X[i];
                temps.Y[i] = stoneTraj.Y[i];
                temps.Vx[i] = stoneTraj.Vx[i];
                temps.Vy[i] = stoneTraj.Vy[i];

                tempg.X[i] = duckTraj.X[i];
            }
            //tempg.Y = duckTraj.Y;
            stoneTraj = temps;
            duckTraj = tempg;
        }

        public void Resolve(int iterationsNumber, double tau)
        {
            stoneTraj = new StoneTrajectory(iterationsNumber);
            duckTraj = new DuckTrajectory(iterationsNumber);

            duckTraj.X[0] = ResolverInputData.x0 + ResolverInputData.dx0;
            stoneTraj.X[0] = ResolverInputData.x0;
            stoneTraj.Y[0] = ResolverInputData.y0;
            stoneTraj.Vx[0] = ResolverInputData.V * Math.Cos(ResolverInputData.alpha * (Math.PI / 180));
            stoneTraj.Vy[0] = ResolverInputData.V * Math.Sin(ResolverInputData.alpha * (Math.PI / 180));

            OdeRungeKutta q = new OdeRungeKutta(new NMPT_StoneFunc());
            double t = 0.0;

            double[] yvars = new double[4];
            yvars[0] = stoneTraj.Vx[0];
            yvars[1] = stoneTraj.Vy[0];
            yvars[2] = stoneTraj.X[0];
            yvars[3] = stoneTraj.Y[0];

            for(int i=1; i<iterationsNumber; i++)
            {
                q.Solve(t, t + tau, yvars);

                if(yvars[3]<0)
                {
                    resizeTraj(i);
                    break;
                }

                stoneTraj.Vx[i] = yvars[0];
                stoneTraj.Vy[i] = yvars[1];
                stoneTraj.X[i] = yvars[2];
                stoneTraj.Y[i] = yvars[3];

                duckTraj.X[i] = duckTraj.X[i - 1] + ResolverInputData.U * tau;

                t += tau;
            }

            findClosestEncounter();
        }

        private void findClosestEncounter()
        {
            double closestX = Math.Abs(stoneTraj.X[0] - duckTraj.X[0]);
            int closestXIndex = 0;
            for(int i=1; i<stoneTraj.X.Length; i++)
            {
                if(Math.Abs(stoneTraj.X[i] - duckTraj.X[i]) < closestX)
                {
                    closestX = Math.Abs(stoneTraj.X[i] - duckTraj.X[i]);
                    closestXIndex = i;
                }
            }

            if(closestX < 1.0)
            {
                duckTraj.Y = stoneTraj.Y[closestXIndex];
            }
        }
    }
}
