using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Media;

namespace gameconsole
{
	public partial class Form1 : Form
	{
		const long MAXJUMPHEIGHT = 25;
		int state;
		Graphics gbuf;
		BufferedGraphics bf;
		BufferedGraphicsContext gc;
		SoundPlayer[] sounds = new SoundPlayer[4];
		

		bool running = false,
			 dead = false;
		long fcount = 0,fsc;
		Font f = new Font("impact", 16),
			 deadfont = new Font("Impact", 60),
			 scorefont = new Font("Impact", 30);
		Brush b = Brushes.White;
		long start_time,score;
		System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
		long sx, sy, sw, sh,delay;
		int[] keys = new int[7];
		int rot_angle, pctr;
		int dir, jh;
		double acc;
		obstacle []o = new obstacle[4];
		simplebg sbg;
		SizeF sdf;
		Bitmap []sprites = new Bitmap[3];
		Bitmap sprite;


		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			gc = BufferedGraphicsManager.Current;
			bf = gc.Allocate(CreateGraphics(), DisplayRectangle);
			gbuf = bf.Graphics;
			timer.Start();
			sprites[0] = new Bitmap(System.Reflection.Assembly.GetEntryAssembly().GetManifestResourceStream("gameconsole.Resources.1.bmp"));
			sprites[1] = new Bitmap(System.Reflection.Assembly.GetEntryAssembly().GetManifestResourceStream("gameconsole.Resources.2.bmp"));
			sprites[2] = new Bitmap(System.Reflection.Assembly.GetEntryAssembly().GetManifestResourceStream("gameconsole.Resources.3.bmp"));
			sounds[0] = new SoundPlayer(System.Reflection.Assembly.GetEntryAssembly().GetManifestResourceStream("gameconsole.Resources.woosh.wav"));
			sounds[1] = new SoundPlayer(System.Reflection.Assembly.GetEntryAssembly().GetManifestResourceStream("gameconsole.Resources.crash.wav"));
			sounds[2] = new SoundPlayer(System.Reflection.Assembly.GetEntryAssembly().GetManifestResourceStream("gameconsole.Resources.thud.wav"));
			sounds[3] = new SoundPlayer(System.Reflection.Assembly.GetEntryAssembly().GetManifestResourceStream("gameconsole.Resources.chime.wav"));
			sprite = sprites[0];
			sw = sprite.Width;
			sh = sprite.Height;
			sprites[0].MakeTransparent(Color.White);
			sprites[1].MakeTransparent(Color.White);
			sprites[2].MakeTransparent(Color.White);
			state = 2;
		}

		void initgame()
		{
			Random qr = new Random();
			dir = 2;
			jh = 0;
			acc = 0.3;
			rot_angle = 0;
			score = 0;

			dead = false;
			for (int q = 0; q < 4; q++)
				o[q] = new obstacle(960 + (q * 192), qr.Next(30, 300), 61);
			sx = (this.DisplayRectangle.Width - sw) / 2;
			sy = (this.DisplayRectangle.Height - sh) / 2;
			sbg = new simplebg();
			sprite = sprites[0];
		}

		private void Form1_Shown(object sender, EventArgs e)
		{
			intro();
		}


		void intro()
		{
			initgame();
			gbuf.Clear(Color.Aquamarine);
			sbg.draw(ref gbuf);
			gbuf.DrawImage(rotatebitmap(sprite, rot_angle), sx, sy);
			putText("THE DUMB BIRD", deadfont, Brushes.BlueViolet , 0, -100);
			putText("Plays like Flappy Bird, use SPACEBAR to control dumb bird.", f, Brushes.HotPink, 0, 100);
			bf.Render();
		}


		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Up)
				keys[0] = 1;
			if (e.KeyCode == Keys.Down)
				keys[1] = 1;
			if (e.KeyCode == Keys.Left)
				keys[2] = 1;
			if (e.KeyCode == Keys.Right)
				keys[3] = 1;
			if (e.KeyCode == Keys.Oemcomma)
				keys[4] = 1;
			if (e.KeyCode == Keys.OemPeriod)
				keys[5] = 1;
			if (e.KeyCode == Keys.Space)
			{
				keys[6] = 1;
				if (!running)
				{
					if (state == 2)
					{
						running = true;
						runit();
					}
					else if (state == 1)
					{
						intro();
						state = 2;
					}
				}
			}
		}

		private void Form1_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Up)
				keys[0] = 0;
			if (e.KeyCode == Keys.Down)
				keys[1] = 0;
			if (e.KeyCode == Keys.Left)
				keys[2] = 0;
			if (e.KeyCode == Keys.Right)
				keys[3] = 0;
			if (e.KeyCode == Keys.Oemcomma)
				keys[4] = 0;
			if (e.KeyCode == Keys.OemPeriod)
				keys[5] = 0;
			if (e.KeyCode == Keys.Space)
				keys[6] = 0;
		}

		Bitmap rotatebitmap(Bitmap b, long angle)
		{
			Bitmap n = new Bitmap(b.Width, b.Height);
			Graphics g = Graphics.FromImage(n);
			g.TranslateTransform((float)b.Width / 2, (float)b.Height / 2);
			g.RotateTransform((float)angle);
			g.TranslateTransform(-(float)b.Width / 2, -(float)b.Height / 2);
			g.DrawImage(b, 0, 0);
			sw = n.Width;
			sh = n.Height;
			return n;
		}

		bool leftmost(long x)
		{
			if (x <= 0) return true;
			return false;
		}

		bool rightmost(long x)
		{
			if ((x + sw) >= 639) return true;
			return false;
		}

		bool topmost(long y)
		{
			if (y <= 0) return true;
			else return false;
		}

		bool bottommost(long y)
		{
			if ((y + sh) >= 479) return true;
			return false;
		}

		bool collide()
		{
			bool rv=false;

			for (int q = 0; q < 4; q++)
			{
				if ((sx + sw) > o[q].x && sx < (o[q].x + o[q].w-2) && (sy + sh) > 0 && sy < o[q].y) rv = true;
				if ((sx + sw) > o[q].x && sx < (o[q].x + o[q].w-2) && (sy + sh) > (o[q].y+150) && sy < 479) rv = true;
			}
			return rv;
		}

		void updatescore()
		{
			for (int q = 0; q < 4; q++)
			{
				if ((sx + sw) > (o[q].x + o[q].w / 2)) 
					if (!o[q].scored)
					{
						score++;
						o[q].scored = true;
						sounds[3].Play();
					}
			}
		}

		void runit()
		{
			initgame();
			start_time = timer.ElapsedMilliseconds;
			while (running)
			{
					if (!dead)
					{
						if (keys[0] == 1 && !topmost(sy))
						{
							acc = 0.3;
							sy -= 2;
						}

						if (keys[1] == 1 && !bottommost(sy))
						{
							sy += 2;
						}
						if (keys[2] == 1 && !leftmost(sx))
						{
							sx -= 2;
						}
						if (keys[3] == 1 && !rightmost(sx))
						{
							sx += 2;
						}
						if (keys[6] == 1)
						{
							if (pctr < 5) pctr++;
							if (pctr == 1)
							{
								sounds[0].Play();
								dir = 1;
								jh = 0;
								acc = 8.0;
								rot_angle = 0;
								sprite = sprites[0];
							}

						}
						else pctr = 0;
					}

					if (dir == 2)
					{
						if (!dead) sprite = sprites[1];
						if (delay > 0) delay--;
						if ((dead && delay == 0) || !dead)
						{
							if (rot_angle < 90)
								if (dead) rot_angle += 3;
								else rot_angle++;
							acc += 0.3;
							if (!bottommost(sy)) sy += (long)acc;
						}
					}
					else
					{
						jh += 2;
						if (jh >= MAXJUMPHEIGHT)
						{
							acc = 0.3;
							dir = 2;
						}
						else
						{
							if (acc > 0.0) acc -= 0.2;
							if (!topmost(sy)) sy -= (long)acc;
							if (rot_angle > -90) rot_angle--;

						}
					}

					gbuf.Clear(Color.Aquamarine);
					sbg.draw(ref gbuf);
					if (!dead) sbg.scroll();
					for (int q = 0; q < 4; q++)
					{
						if (!dead) o[q].move();
						o[q].draw(ref gbuf);
					}
					gbuf.DrawImage(rotatebitmap(sprite, rot_angle), sx, sy);
					sdf = gbuf.MeasureString(score.ToString(), scorefont);
					gbuf.DrawString(score.ToString(), scorefont, Brushes.Black, 2+(50 - sdf.Width) / 2, 12);
					gbuf.DrawString(score.ToString(), scorefont, Brushes.Gold, (50 - sdf.Width) / 2, 10);
					updatescore();
					if (collide())
					{
						if (!dead)
						{
							sounds[1].Play();
							delay = 25;
							fsc = 0;
							rot_angle = 0;
							sprite = sprites[2];
							dir = 2;
						}
						dead = true;
					}
					if (bottommost(sy))
					{
						if (!dead)
						{
							sounds[2].Play();
							delay = 25;
							fsc = 0;
							sprite = sprites[2];
						}
						dead = true;
						putText("DEAD BIRD", deadfont, Brushes.Yellow, 0, 0);
						if (score > 0 && fsc < score)
						{
							sounds[0].Play();
							fsc++;
						}
						putText("Your Score:", scorefont, Brushes.HotPink, 0, 100);
						putText(fsc.ToString(), scorefont, Brushes.DarkOrange, 0, 150);
						if (fsc == score)
							running = false;
						state = 1;
						start_time += 33;
					}

					fcount++;
					bf.Render();
					start_time += 16;
					
					while (start_time > timer.ElapsedMilliseconds)
							Application.DoEvents();

			}
		}
		void putText(String s,Font f,Brush b,int ox,int oy)
		{
			SizeF z;
			z = gbuf.MeasureString(s,f);
			gbuf.DrawString(s, f, Brushes.Black, ox +2+ (DisplayRectangle.Width - z.Width) / 2, oy + 2+(DisplayRectangle.Height - z.Height) / 2);
			gbuf.DrawString(s, f, b, ox+(DisplayRectangle.Width - z.Width) / 2, oy+(DisplayRectangle.Height - z.Height) / 2);
		}
	}

	class obstacle
	{
		public float x, y, w, startx;
		Random r;
		public bool scored = false;

		public obstacle(float xi, float yi, float c)
		{
			x = xi;
			startx = xi;
			y = yi;
			w = c;
			r = new Random();
		}

		public void draw(ref Graphics g)
		{
			if (!scored)
			{
				g.FillRectangle(Brushes.Red, x - 6, 0, w + 6, y);
				g.FillRectangle(Brushes.Red, x - 6, y + 150, w + 6, 329 - y);
			}
			else
			{
				g.FillRectangle(Brushes.Green, x - 6, 0, w + 6, y);
				g.FillRectangle(Brushes.Green, x - 6, y + 150, w + 6, 329 - y);
			}
			g.DrawRectangle(Pens.Black, x - 6, 0, w + 6, y);
			g.DrawRectangle(Pens.Black, x - 6, y + 150, w + 6, 329 - y);

		}

		public void move()
		{
			r = new Random();
			if (x <= -132)
			{
				x = 640;
				y = r.Next(30, 250);
				scored = false;
			}
			x -= 2;
		}
	}

	public class simplebg
	{
		long x;

		public simplebg()
		{
			x = 0;
		}
		public void scroll()
		{
			x--;
			if (x <= -640) x = 0;
		}

		public void draw(ref Graphics g)
		{
			Point[] p = new Point[3];
			for (long u = x; u <= (x + 1280); u += 128)
			{
				p[0].X = (int)u;
				p[0].Y = 430;
				p[1].X = (int)u+75;
				p[1].Y = 400;
				p[2].X = (int)u+128;
				p[2].Y = 430;
				g.FillPolygon(Brushes.ForestGreen, p);
				g.FillRectangle(Brushes.LightSeaGreen , u, 430, 128, 79);
				g.DrawRectangle(Pens.DarkGreen, u, 430, 128, 79);
				g.FillEllipse(Brushes.LightGray , u , 100, 135, 40);
				g.FillEllipse(Brushes.White, u, 50, 140, 85);
			}
		}
	}


}

