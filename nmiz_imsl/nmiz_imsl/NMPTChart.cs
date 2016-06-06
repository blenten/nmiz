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
        //double[] y, x;
        public NMPTChart(double[] X, double[] Y)
        {
            Chart chart = this.Chart;
            AxisXY axis = new AxisXY(chart);
            new Data(axis, X, Y);
        }

        public static void MakeChar(double[] X, double[] Y)
        {
            NMPTChart nmptc = new NMPTChart(X, Y);
            nmptc.Width = 600;
            nmptc.Height = 600;
            nmptc.ShowDialog();
        }
    }
}
