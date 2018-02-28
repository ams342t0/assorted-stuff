import java.util.*;

class q
{
	int qq=10000000;


	public void mergesort(int []a,int la,int []b,int lb)
	{
		int ma,mb,ia=0,ib=0,k=0;
		int []tmp;
		if (la > 1)
		{
			ma = la >> 1;
			int []ta1;
			ta1=new int[la-ma];
			System.arraycopy(a,ma,ta1,0,la-ma);
			mergesort(a,ma,ta1,la-ma);
		}
		if (lb > 1)
		{
			mb = lb >> 1;
			int []ta1;
			ta1=new int[lb-mb];
			System.arraycopy(b,mb,ta1,0,lb-mb);
			mergesort(b,mb,ta1,lb-mb);
		}
		tmp = new int[la+lb];
		while (ia < la && ib < lb)
		{
			if (a[ia] <= b[ib])
			{
				tmp[k]=a[ia];
				ia++;
			}
			else
			{
				tmp[k]=b[ib];
				ib++;
			}
			k++;
		}

		while (ia < la)
		{
			tmp[k]=a[ia];
			ia++;
			k++;
		}
		while (ib<lb)
		{
			tmp[k]=b[ib];
			ib++;
			k++;
		}

		for(k=0;k<(ia+ib);k++)
			a[k]=tmp[k];
	}


	public void startit()
	{
		Random r= new Random();
		int []s=new int[qq];
		int []x=new int[qq-1];
		long t;

		for(int y=0;y<qq;y++)
			s[y] = Math.round(r.nextInt(1048576));
		
		
		System.arraycopy(s,1,x,0,qq-1);
				
		t = System.nanoTime();
		mergesort(s,1,x,qq-1);

		System.out.printf("\nDone %4.2f",(float)((System.nanoTime()-t)/1000000000.0));

//		for(int y=0;y<qq;y++)
	//		System.out.printf("\n%d",s[y]);

	}


	public static void main(String[]v)
	{
		q n = new q();

		n.startit();
	}
}