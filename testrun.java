import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.Color;
import java.util.ArrayList;
import java.applet.Applet;

public class testrun extends Applet implements Runnable
{
	BufferedImage s,i;
	Graphics2D gx,g1;
	double[][] cangles;
	explosion[] expfx=new explosion[20];
	int expctr=0,expcued=0;

	int y;
	long ti;

	class explosion
	{
		int distance;
		double tgx,tgy;
		int mstatus;
		int crad=4;
		int radctr=2;
		int cx,cy;


		public explosion(int x,int y)
		{
			distance=0;
			mstatus=1;
			cx = x;
			cy = y;
			
		}

		public void renderexplosion(Graphics g1)
		{
			if (mstatus==1)
			{			
				g1.setColor(Color.white);

				g1.drawOval(cx-(crad/2),cy-(crad/2)-distance,crad,crad);
				g1.drawOval(cx-(crad/2),cy-(crad/2)+distance,crad,crad);
				g1.drawOval(cx-(crad/2)-distance,cy-(crad/2),crad,crad);
				g1.drawOval(cx-(crad/2)+distance,cy-(crad/2),crad,crad);

				for(int aix=0;aix<3;aix++)
				{
					tgx = distance*cangles[aix][0];
					tgy = distance*cangles[aix][1];
					g1.drawOval(cx+(int)tgx-(crad/2),cy+(int)tgy-(crad/2),crad,crad);
					g1.drawOval(cx-(int)tgx-(crad/2),cy+(int)tgy-(crad/2),crad,crad);
					g1.drawOval(cx+(int)tgx-(crad/2),cy-(int)tgy-(crad/2),crad,crad);
					g1.drawOval(cx-(int)tgx-(crad/2),cy-(int)tgy-(crad/2),crad,crad);

				}


				distance+=2;
				if (distance>=300)
				{
					distance = 0;
					mstatus = 0;
				}

				crad+=radctr;
				
				if (crad == 20 || crad == 4) radctr=-radctr;
			}

		}

	}

	@Override
	public void start()
	{
		cangles = new double[4][2];
		cangles[0][0]=Math.sin(22.5*Math.PI/180);
		cangles[0][1]=Math.cos(22.5*Math.PI/180);

		cangles[1][0]=Math.sin(45*Math.PI/180);
		cangles[1][1]=Math.cos(45*Math.PI/180);

		cangles[2][0]=Math.sin(67.5*Math.PI/180);
		cangles[2][1]=Math.cos(67.5*Math.PI/180);

		addMouseListener(new MouseListener()
		{
			public void mousePressed(MouseEvent e){}
			public void mouseReleased(MouseEvent e){}
			public void mouseEntered(MouseEvent e){}
			public void mouseExited(MouseEvent e){}
			public void mouseClicked(MouseEvent e)
			{
				if (e.getID()==MouseEvent.MOUSE_CLICKED)
				{
					startexplosion(e.getX(),e.getY());

				}
			}
		});
		i = new BufferedImage(800,600,BufferedImage.TYPE_INT_RGB);
		gx = (Graphics2D)getGraphics();
		g1 = (Graphics2D)i.getGraphics();
		s = new BufferedImage(8,8,BufferedImage.TYPE_INT_RGB);
		
		enableEvents(8);
		new Thread(this).start();
	}


	void startexplosion(int x,int y)
	{
					expfx[expctr]=new explosion(x,y);
					expctr++;
					expcued++;
					if (expctr>19) 
					{
						expctr=0;
					}
					if (expcued>19) expcued=19;

	}

	public void run()
	{
		long tdiff,tstart;


 		y=0;
		ti=System.nanoTime();
		while (true)
		{
			tstart = System.nanoTime();

			if (y++ > 300) 
			{
				startexplosion(y,50);
				y=0;

			}

			g1.setColor(Color.black);
			g1.fillRect(0,0,800,600);

			g1.setColor(Color.red);
			g1.fillRect(0,y,150,50);

			for (int ax=0;ax<8;ax++)
				for (int ay=0;ay<8;ay++)
					s.setRGB(ax,ay,0xff00ff);

			g1.drawImage(s,y,50,null);

			g1.setColor(Color.green);
			g1.drawString("Count: " +expctr,10,50);

			
			for(int iex=1;iex<=expcued;iex++)
			{
				expfx[iex-1].renderexplosion(g1);
			}

			gx.drawImage(i,0,0,800,600,null);	


			ti+=16666667;

			while ((ti-System.nanoTime())>0)
			{
				Thread.yield();
			}
		}	
	}

	@Override
	public void processKeyEvent(KeyEvent ke)
	{
		System.out.println(ke.getID()+" "+ke.getKeyCode());
	}


	public static void main(String[] v)
	{
		JFrame f;

		testrun a=new testrun();

		f=new JFrame("Graphics test");
		a.setPreferredSize(new Dimension(800,600));
		f.setVisible(true);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setSize(800,600);
		f.add(a);
		f.pack();
		a.start();

	}
}