﻿using Imsl.Math;

namespace nmpt_quadr
{
    class NMptQuadr
    {
        public static void minimise()
        {
            double[,] A =
            {
                { 4.65955429E-01, 6.70777929E-01, 6.04621091E-01,-3.99452683E-02, -2.45113745E-01, -2.08470325E-01},
                { 6.70777929E-01, 1.16048933E+00, 8.68797320E-01, -1.24379806E-01, -2.79029766E-01, -2.64101950E-01},
                { 6.04621091E-01,  8.68797320E-01, 1.74606644E+00, -2.99226080E-01, -4.29243219E-01, -5.45930152E-01 },
                { -3.99452683E-02, -1.24379806E-01, -2.99226080E-01, 9.32310097E-01, 6.88144081E-01, 1.35320067E+00},
                {-2.45113745E-01, -2.79029766E-01, -4.29243219E-01, 6.88144081E-01, 7.66772995E-01, 1.17955535E+00},
                {-2.08470325E-01, -2.64101950E-01, -5.45930152E-01, 1.35320067E+00, 1.17955535E+00, 2.15814681E+00}
            };

            double[] b = { 5.15128821E-01, 6.58621484E-01, 8.60595339E-01, 7.81968084E-01, 7.29632555E-01, 7.16546190E-01 };

            double[,] hessian =
            {
                { 2*A[0,0], A[0,1] + A[1,0], A[0,2] + A[2,0], A[0,3] + A[3,0], A[0,4] + A[4,0], A[0,5] + A[5,0] },
                { A[0,1] + A[1,0], 2*A[1,1], A[1,2] + A[2,1], A[1,3] + A[3,1], A[1,4] + A[4,1], A[1,5] + A[5,1] },
                { A[0,2] + A[2,0], A[1,2] + A[2,1], 2*A[2,2], A[2,3] + A[3,2], A[2,4] + A[4,2], A[2,5] + A[5,2] },
                { A[0,3] + A[3,0], A[1,3] + A[3,1], A[2,3] + A[3,2], 2*A[3,3], A[3,4] + A[4,3], A[3,5] + A[5,3] },
                { A[0,4] + A[4,0], A[1,4] + A[4,1], A[2,4] + A[4,2], A[3,4] + A[4,3], 2*A[4,4], A[4,5] + A[5,4] },
                { A[0,5] + A[5,0], A[1,5] + A[5,1], A[2,5] + A[5,2], A[3,5] + A[5,3], A[4,5] + A[5,4], 2*A[5,5] },
            };

            double[,] equality =
            {
                { 1,1,1,1,1,1 },
                { b[0], b[1], b[2], b[3], b[4], b[5] }
            };

            double[] bEquality = { 1, 0.8 };

            double[,] inequality =
            {
                { 0, 0, 0, 0, 0, 1 },
                { 0, 0, 0, 0, 0, -1 },
                { 0, 1, 0, 1, 0, 0 },
                { 0, -1, 0, -1, 0, 0 },
                { 1, 0, 1, 0, 1, 0 },
                { -1, 0, -1, 0, -1, 0 }
            };

            double[] bInequality = { 0.2, -0.14, 0.5, -0.35, 0.6, -0.3 };

            double[] g = { 0, 0, 0, 0, 0, 0 };

            QuadraticProgramming qp = new QuadraticProgramming(hessian, g, equality, bEquality, inequality, bInequality);

            new PrintMatrix("Solution").Print(qp.GetSolution());
            new PrintMatrix("Lagrange multipliers").Print(qp.GetDualSolution());

            System.Console.ReadLine();
        }
    }
}