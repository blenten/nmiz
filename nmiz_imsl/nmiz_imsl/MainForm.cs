using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace nmiz_imsl
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            StreamWriter writer = new StreamWriter("output.txt");

            NMPTResolver nmpt = new NMPTResolver();
            nmpt.Resolve(500, 0.001);

            for(int i=0; i<500; i++)
            {
                writer.WriteLine(String.Format("{0}\t{1}\t{2}\t{3}", nmpt.StoneX[i], nmpt.StoneY[i], nmpt.StoneVx[i], nmpt.StoneVy[i]));
            }            

            InitializeComponent();
        }
    }
}
