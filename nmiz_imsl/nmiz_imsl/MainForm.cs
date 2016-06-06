using System;
using System.Windows.Forms;
using System.IO;

namespace nmiz_imsl
{
    public partial class MainForm : Form
    {

        private NMPTResolver nmpt;
        public MainForm()
        {
            InitializeComponent();

            UBox.Text = ResolverInputData.U.ToString();
            VBox.Text = ResolverInputData.V.ToString();
            dX0Box.Text = ResolverInputData.dx0.ToString();
            alphaBox.Text = ResolverInputData.alpha.ToString();
            gBox.Text = ResolverInputData.g.ToString();
            kgBox.Text = ResolverInputData.kg.ToString();
            ksvBox.Text = ResolverInputData.ksv.ToString();
            mBox.Text = ResolverInputData.m.ToString();
            tauBox.Text = "0,001";
            iterBox.Text = "1000";
        }
        public void saveData()
        {
            StreamWriter writer = new StreamWriter("output.txt");
            writer.WriteLine("Stone (X, Y, Vx, Vy)");
            for(int i=0; i< nmpt.StoneX.Length; i++)
            {
                writer.WriteLine(String.Format("{0:0.0000000}\t{1:0.0000000}\t{2:0.0000000}\t{3:0.0000000}",
                    nmpt.StoneX[i], nmpt.StoneY[i], nmpt.StoneVx[i], nmpt.StoneVy[i]));
            }

            writer.WriteLine("\n\n");
            writer.WriteLine("Duck (X)");

            for (int i = 0; i < nmpt.DuckX.Length; i++)
            {
                writer.WriteLine(String.Format("{0:0.0000000}",
                   nmpt.DuckX[i]));
            }

            if (nmpt.PossibleDuckY==-1)
            {
                writer.WriteLine("Possible duck Y: unknown");
            }
            else
            {
                writer.WriteLine(String.Format("Possible duck Y: {0}", nmpt.PossibleDuckY));
            }
            writer.Close();
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            Double.TryParse(UBox.Text, out ResolverInputData.U);
            Double.TryParse(VBox.Text, out ResolverInputData.V);
            Double.TryParse(dX0Box.Text, out ResolverInputData.dx0);
            Double.TryParse(gBox.Text, out ResolverInputData.g);
            Double.TryParse(kgBox.Text, out ResolverInputData.kg);
            Double.TryParse(ksvBox.Text, out ResolverInputData.ksv);
            Double.TryParse(alphaBox.Text, out ResolverInputData.alpha);
            Double.TryParse(mBox.Text, out ResolverInputData.m);
            int iters;
            int.TryParse(iterBox.Text, out iters);
            double tau;
            Double.TryParse(tauBox.Text, out tau);

            nmpt = new NMPTResolver();
            nmpt.Resolve(iters, tau);

            if(checkSave.Checked)
            {
                saveData();
            }

            NMPTChart.MakeChar(nmpt.StoneX, nmpt.StoneY);
        }

        private void buttonExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
