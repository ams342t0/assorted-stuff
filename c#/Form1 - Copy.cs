using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AForge.Imaging.Filters;
using AForge.Video;
using AForge.Video.DirectShow;
using System.Diagnostics;
using System.IO;
using AForge;

namespace quickOCR
{
    public partial class Form1 : Form
    {
        FilterInfoCollection fic;
        VideoCaptureDevice vcd;
        Bitmap cam_bmp,sel_bmp;
        double x1, y1, x2, y2,sx,sy;
        double fx, fy;
        BufferedGraphics bg;
        BufferedGraphicsContext bgc;
        ContrastCorrection cc;
        BrightnessCorrection bc;
        Grayscale gs;
        bool snap_ready=false;
        int frame_count = 0;
        Graphics g;
        Pen grid, corner_line;
        QuadrilateralTransformation qt;
        System.Drawing.Point[] corners;
        int corner_index=-1;

        public Form1()
        {
            InitializeComponent();
            fic = new FilterInfoCollection(FilterCategory.VideoInputDevice);
            bgc = BufferedGraphicsManager.Current;
            cc = new ContrastCorrection((int)nContrast.Value);
            bc = new BrightnessCorrection((int)nBrightness.Value);
            grid = new Pen(Brushes.LimeGreen, 2);
            sel_rect.Checked = true;
            cbCamera.Items.Clear();
            corners = new System.Drawing.Point[4];

            foreach (FilterInfo f in fic)
            {
                cbCamera.Items.Add(f.Name);
            }
            if (cbCamera.Items.Count > 0)
                cbCamera.SelectedIndex = 0;
        }

        void vcd_NewFrame(Object s, NewFrameEventArgs a)
        {
            using (cam_bmp = (Bitmap)a.Frame.Clone())
            {
                cam_bmp = bc.Apply(cam_bmp);
                cam_bmp = cc.Apply(cam_bmp);
                camview.Image = cam_bmp;
                using (g = camview.CreateGraphics())
                {
                    fx = (float)cam_bmp.Width / (float)camview.Width;
                    fy = (float)cam_bmp.Height / (float)camview.Height;
                    g.DrawLine(grid, camview.Width / 2, 0, camview.Width / 2, camview.Height - 1);
                    g.DrawLine(grid, 0, camview.Height / 2, camview.Width - 1, camview.Height / 2);
                }
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                vcd.Stop();
            }
            catch (Exception ex)
            {
            }
        }

        private void camview_MouseDown(object sender, MouseEventArgs e)
        {
            if (!snap_ready)
            {
                x1 = e.X;
                y1 = e.Y;

                if (sel_corners.Checked)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (e.X >= corners[i].X - 5 && e.X <= corners[i].X + 5 &&
                            e.Y >= corners[i].Y - 5 && e.Y <= corners[i].Y + 5)
                        {
                            corner_index = i;
                            break;
                        }

                    }
                }

                bg = bgc.Allocate(camview.CreateGraphics(), camview.DisplayRectangle);
            }
        }

        private void camview_MouseUp(object sender, MouseEventArgs e)
        {
            int select_width, select_height;

            if ((e.X == x1) || (e.Y == y1))
                return;

            if (!snap_ready)
            {

                if (sel_rect.Checked)
                {
                    x2 = e.X;
                    y2 = e.Y;

                    if (x2 <= 0)
                        x2 = 0;

                    if (x2 >= camview.Width - 1)
                        x2 = camview.Width - 1;

                    if (y2 <= 0)
                        y2 = 0;

                    if (y2 >= camview.Height - 1)
                        y2 = camview.Height - 1;


                    if (x1 < x2)
                        sx = x1;
                    else
                        sx = x2;

                    if (y1 < y2)
                        sy = y1;
                    else
                        sy = y2;

                    select_width = (int)Math.Round((fx * (1 + Math.Abs(x1 - x2))), 0);
                    select_height = (int)Math.Round((fy * (1 + Math.Abs(y1 - y2))), 0);
                    sx = Math.Round((sx * fx), 0);
                    sy = Math.Round((sy * fy), 0);

                    sel_bmp = cam_bmp.Clone(new Rectangle((int)sx, (int)sy, select_width, select_height), cam_bmp.PixelFormat);

                    selection.Image = sel_bmp;
                    richText.Text = "Ungarbling this gibberish...";
                    process_image();
                }
                else
                {
                    List<IntPoint> quad;
                    quad = new List<IntPoint>(4);
                    qt = new QuadrilateralTransformation();
                    quad.Add(new IntPoint((int)(fx*corners[0].X),(int)(fy*corners[0].Y)));
                    quad.Add(new IntPoint((int)(fx * corners[1].X),(int)(fy* corners[1].Y)));
                    quad.Add(new IntPoint((int)(fx * corners[2].X),(int)(fy* corners[2].Y)));
                    quad.Add(new IntPoint((int)(fx * corners[3].X),(int)(fy* corners[3].Y)));
                    qt.SourceQuadrilateral = quad;
                    sel_bmp = qt.Apply(cam_bmp);
                    selection.Image = sel_bmp;
                    corner_index = -1;
                }
            }
        }


        private void camview_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left && !snap_ready)
            {
                if (sel_rect.Checked)
                {
                    x2 = e.X;
                    y2 = e.Y;

                    if (x1 < x2)
                        sx = x1;
                    else
                        sx = x2;

                    if (y1 < y2)
                        sy = y1;
                    else
                        sy = y2;


                    try
                    {
                        bg.Graphics.DrawImage(cam_bmp, camview.DisplayRectangle);
                        bg.Graphics.DrawRectangle(new Pen(Brushes.Red, 2), (int)sx, (int)sy, (int)(1 + Math.Abs((x2 - x1))), (int)(1 + Math.Abs((y2 - y1))));
                        bg.Render();
                    }
                    catch (Exception ex)
                    {
                    }
                }
                else
                {
                    bg.Graphics.DrawImage(cam_bmp, camview.DisplayRectangle);
                    if (corner_index > -1)
                    {
                        corners[corner_index].X = e.X;
                        corners[corner_index].Y = e.Y;
                    }
                    draw_corner_select();
                    bg.Render();
                }
            }

            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                float centerx, centery;
                centerx = camview.Width / 2;
                centery = camview.Height / 2;
            }
        }

        void process_image()
        {
            if (File.Exists("output.txt"))
                File.Delete("output.txt");
            sel_bmp.Save(@"q.jpg");
			Process p = new Process();
			p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            p.StartInfo.FileName = Path.Combine("tesseract.exe");
			p.StartInfo.Arguments = "q.jpg output";
            //p.Start();
            //p.WaitForExit();
            //richText.Text = File.ReadAllText("output.txt");
        }

        void printstatus()
        {
            richText.Text = x1.ToString() + "," + y1.ToString() + " - " + x2.ToString() + "," + y2.ToString() + "\n" +
                "fx: " + fx.ToString() + ", fy: " + fy.ToString() + "\n" +
                camview.Width.ToString() + "x" + camview.Height.ToString() + "\n" +
                cam_bmp.Width.ToString() + "x" + cam_bmp.Height.ToString();
        }

        private void cmdCopy_Click(object sender, EventArgs e)
        {
            try
            {
                Clipboard.SetText(richText.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            nBrightness.Value = 18;
            nContrast.Value = 30;
            setadjust();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            nBrightness.Value = -30;
            nContrast.Value = 60;
            setadjust();
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            nBrightness.Value = -60;
            nContrast.Value = 100;
            setadjust();
        }

        void setadjust()
        {
            cc = new ContrastCorrection((int)nContrast.Value);
            bc = new BrightnessCorrection((int)nBrightness.Value);
        }

        private void nBrightness_ValueChanged(object sender, EventArgs e)
        {
            setadjust();
        }

        private void nContrast_ValueChanged(object sender, EventArgs e)
        {
            setadjust();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                Clipboard.SetImage(selection.Image);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnAppend_Click(object sender, EventArgs e)
        {
            if (richText.Text.Length > 0)
            {
                txtCompiledText.Text += "\n" + richText.Text;
            }
        }

        private void xCAPTUREToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (snap_ready)
                return;

            snap_ready = true;
            vcd = new VideoCaptureDevice(fic[cbCamera.SelectedIndex].MonikerString);
            vcd.VideoResolution = vcd.VideoCapabilities[cbRes.SelectedIndex];
            vcd.NewFrame += new NewFrameEventHandler(vcd_NewFrame);
            vcd.Start();
            camview.SizeMode = PictureBoxSizeMode.StretchImage;
            Text = "";
        }

        private void sNAPToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!snap_ready)
                return;

            snap_ready = false;
            vcd.Stop();
        }

        private void cOPYTEXTToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                Clipboard.SetText(richText.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void cOPYIMAGEToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                Clipboard.SetImage(selection.Image);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void aPPENDToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (richText.Text.Length > 0)
            {
                txtCompiledText.Text += " " + richText.Text;
            }

        }

        private void lOADToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (snap_ready == true)
                return;

            openFileDialog1.FileName = "";
            openFileDialog1.Multiselect = false;
            openFileDialog1.Filter = "JPEG|*.JPG|JPEG|*.JPEG|PNG|*.PNG|GIF|*.GIF|BITMAP|*.BMP";
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                cam_bmp = new Bitmap(openFileDialog1.FileName);
                camview.Image = cam_bmp;
                camview.SizeMode = PictureBoxSizeMode.StretchImage;
                fx = (float)cam_bmp.Width / (float)camview.Width;
                fy = (float)cam_bmp.Height / (float)camview.Height;
                Text = openFileDialog1.FileName;
            }
        }

        private void camview_Resize(object sender, EventArgs e)
        {
            if (cam_bmp != null)
            {
                fx = (float)cam_bmp.Width / (float)camview.Width;
                fy = (float)cam_bmp.Height / (float)camview.Height;
            }
        }

        private void cLEANUPToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (richText.SelectedText.Length > 0)
            {
                richText.SelectedText = richText.SelectedText.Replace('\n', ' ').TrimEnd(' ','\n').Replace("ﬁ","fi").Replace("ﬂ","fl");
            }
        }

        private void cOPYAPPENDEDToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                Clipboard.SetText(txtCompiledText.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void cLEANCOPYToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (richText.Text.Length > 0)
            {
                richText.Text = richText.Text.Replace('\n', ' ').TrimEnd(' ', '\n').Replace("ﬁ", "fi").Replace("ﬂ", "fl");
            }
            try
            {
                Clipboard.SetText(richText.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void cbCamera_SelectedIndexChanged(object sender, EventArgs e)
        {
            cbRes.Items.Clear();
            VideoCaptureDevice v;
            
            v = new VideoCaptureDevice(fic[cbCamera.SelectedIndex].MonikerString);
            foreach (VideoCapabilities vc in v.VideoCapabilities)
            {
                cbRes.Items.Add(vc.FrameSize.Width.ToString() + " x " + vc.FrameSize.Height.ToString());
            }
            cbRes.SelectedIndex = 0;
        }


        private void sel_corners_CheckedChanged(object sender, EventArgs e)
        {
            bg = bgc.Allocate(camview.CreateGraphics(), camview.DisplayRectangle);            
            bg.Graphics.DrawImage(cam_bmp, camview.DisplayRectangle);
            
            corners[0].X = camview.DisplayRectangle.Width/4;
            corners[0].Y = camview.DisplayRectangle.Height/4;
            corners[1].X  =3 * camview.DisplayRectangle.Width / 4;
            corners[1].Y = camview.DisplayRectangle.Height / 4;
            corners[2].X = 3 * camview.DisplayRectangle.Width / 4;
            corners[2].Y = 3 * camview.DisplayRectangle.Height / 4;
            corners[3].X = camview.DisplayRectangle.Width / 4;
            corners[3].Y = 3 * camview.DisplayRectangle.Height / 4;
            draw_corner_select();
            bg.Render();
        }

        void draw_corner_select()
        {
            corner_line = new Pen(Brushes.Green, 2);
            bg.Graphics.DrawLine(corner_line, corners[0].X, corners[0].Y, corners[1].X, corners[1].Y);
            bg.Graphics.DrawLine(corner_line, corners[1].X, corners[1].Y, corners[2].X, corners[2].Y);
            bg.Graphics.DrawLine(corner_line, corners[2].X, corners[2].Y, corners[3].X, corners[3].Y);
            bg.Graphics.DrawLine(corner_line, corners[3].X, corners[3].Y, corners[0].X, corners[0].Y);
            bg.Graphics.DrawEllipse(corner_line, corners[0].X - 5, corners[0].Y - 5, 10, 10);
            bg.Graphics.DrawEllipse(corner_line, corners[1].X - 5, corners[1].Y - 5, 10, 10);
            bg.Graphics.DrawEllipse(corner_line, corners[2].X - 5, corners[2].Y - 5, 10, 10);
            bg.Graphics.DrawEllipse(corner_line, corners[3].X - 5, corners[3].Y - 5, 10, 10);
        }

    }
}
