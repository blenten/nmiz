using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Imsl.Chart2D;

namespace nmiz_imsl
{
    class NMPTChart : FrameChart
    {
        AxisXY axis;
        public NMPTChart()
        {
            Chart chart = this.Chart;
            axis = new AxisXY(chart);
        }

        public void drawStoneTraj(double[] X, double[] Y)
        {
            new Data(axis, X, Y);
        }
        public void drawDuckTraj(double Y, int len)
        {
            double[] y = new double[len];
            for(int i=0; i<y.Length; i++)
            {
                y[i] = Y;
            }
            new Data(axis, y);
        }

        public static void MakeChar(double[] X, double[] Y, double duckY=-1)
        {
            NMPTChart nmptc = new NMPTChart();
            nmptc.Width = 800;
            nmptc.Height = 300;
            nmptc.drawStoneTraj(X, Y);
            if(duckY>=0)
            {
                nmptc.drawDuckTraj(duckY, X.Length);
            }
            nmptc.ShowDialog();
        }
    }
}
