/*

expands a polynomial raised to n
using binomial theorem

*/


public class bt
{
	public static long n;
	public static String a,b;


	public static long fact(long n)
	{

		if (n<=0) return 1;


		return (n*fact(n-1));

	}

	public static long tfact(long n,long i)
	{

		if (n<=i) return 1;
		
		return (n*tfact(n-1,i));
		
	}

	public static long p(long i,long n)
	{
		return (tfact(i,i-n)/fact(n));
	}

	public static String pterm(String a,String b,long x,long n)
	{
		String r="";
		long c;

		c = p(x,(x-n));
		
		if (c>1)	
			r+=c;
	

		if (n>0)
		{
			r += a;
			
			if (n>1) r+= "^" + n;
			
		}
		
		if ((x-n)>0)
		{
			r += b;
			if ((x-n)>1) r += "^" +(x-n);
		}

		return r;
		
	}
	

	public static void main(String []v)
	{
		long u,c,x;
		String pt;

		pt = new String();

		System.out.println(v.length);

		if (v.length == 3)
		{
			try
			{
				n = Long.parseLong(v[0]);
				a = v[1];
				b = v[2];
			}
			catch(Exception e)
			{
				n = 6;
				a = "X";
				b = "Y";
			}
		}

		x = n;
		
		pt="";

		while (n>=0)
		{
			pt += pterm(a,b,x,n);
			n--;

			if (n>=0) pt+="+";
		}				

		System.out.println(pt);
	}
	

}