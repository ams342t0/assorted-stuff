using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AForge.Imaging;
using System.Windows.Media;
using AForge.Imaging.Filters;
using AForge;
using AForge.Imaging;

namespace bg_review
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Graphics g;
            Bitmap b;
            Rectangle r;
            System.Drawing.Pen p;
            float x, space;
            QuadrilateralTransformation qt;
            List<IntPoint> quad;
            qt = new QuadrilateralTransformation();
            
            
            
            b = new Bitmap(@"d:\index.jpg");
            g = Graphics.FromImage(b);
            r = new Rectangle(new System.Drawing.Point(0,0),b.Size);
            p = new System.Drawing.Pen(System.Drawing.Brushes.Blue, 10);


            quad = new List<IntPoint>();

            quad.Add(new IntPoint(88,92));
            quad.Add(new IntPoint(309,105));
            quad.Add(new IntPoint(305,292));
            quad.Add(new IntPoint(91,305));

            qt.SourceQuadrilateral = quad;
            b = qt.Apply(b);

            space = b.Width / 3;
            x = 0f;
            for (int i = 0; i < 4; i++)
            {
                g.DrawLine(p, x,0f,x, b.Height);
                x += space;
            }
            
            pictureBox1.Image = b;
        }
    }
}
