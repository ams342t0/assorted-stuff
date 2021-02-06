class jcalc{
   
    class expBuffer
    {
        String []ops;
        String [] values;
        int opCount;
        int valCount;
        
        expBuffer()
        {
            ops = new String[1024];
            values = new String[1024];
            opCount = 0;
            valCount = 0;
        }
        
        void putVal(String v, int pos)
        {
            values[pos] = v;
        }
        
        void pushVal(String v)
        {
            values[valCount] = v;
            valCount++;
        }
        
        void pushOp(String o)
        {
            ops[opCount] = o;
            opCount++;
        }
        
        String peekOp(int pos)
        {
            return ops[pos];
        }

        String peekVal(int pos)
        {
            return values[pos];
        }
        
        String popOp(int pos)
        {
            String retval="";
            retval = ops[pos];
            
            if (pos < (opCount-1))
            {
                for(int x = pos;x<opCount-1;x++)
                {
                    ops[x] = ops[x+1];
                }
            }
            opCount--;
            return retval;
        }
        
        String popVal(int pos)
        {
            String retval="";
            retval = values[pos];

            if (pos < (valCount-1))
            {
                for(int x = pos;x<valCount-1;x++)
                {
                    values[x] = values[x+1];
                }
            }
            valCount--;
            return retval;
        }

        void dump()
        {
            System.out.printf("\nOps : %d, Values : %d",opCount,valCount);
            for(int x = 0;x<opCount;x++)
                System.out.printf("\n%s",ops[x]);

            for(int x = 0;x<valCount;x++)
                System.out.printf("\n%s",values[x]);
		}
        
		/*
        void splitExpression() throws Exception
        {
            values = exp.split("[\\*\\/\\+\\-]");
            ops = exp.split("[ \\.0-9]+");
            valCount = values.length;
            opCount = ops.length;
            popOp(0);
        }*/
    }
    
    double reduce(String input) throws Exception
        {
			expBuffer b;
			String tmpstr="",ops,num,par,s;
			char []bs;
			int delim=-1,lastdelim=-1,ve=-1,vs=-1,ps=-1,pe=-1,pt=-1;
            double retval=0.0,tmp=0.0;
            int i;
			String[]operators={"*/","+-"};

			b = new expBuffer();
			bs = input.toCharArray();
			ops = "*/+-\n";
			num = "0123456789.";
			par = "(";

			i = 0;
			do
			{
				if (ops.contains(""+bs[i]))
					lastdelim = 0;
				
				if (num.contains(""+bs[i]))
					lastdelim=1;

				if (par.contains(""+bs[i]))
					lastdelim=2;
				
				if (delim != lastdelim)
				{
					switch (lastdelim)
					{
						case 0:
							if (bs[i]!='\n')
								b.pushOp(""+bs[i]);
							
							if (vs>=0)
								b.pushVal(tmpstr);
							tmpstr="";	
							break;
							
						case 1:
							tmpstr += "" + bs[i];
							vs = i;
							break;
							
						case 2:
							ps = i;
							i++;
							int q=i;
							pt = 1;
							while (pt>0)
							{
								if (bs[q]=='(')
									pt++;
								if (bs[q]==')')
									pt--;
								q++;
								i++;
							}
							i--;
							vs=-1;
							tmpstr="";
							b.pushVal(Double.toString(reduce(input.substring(ps+1,q-1)+'\n')));
							break;
					}
					delim = lastdelim;
				}
				else
				{
					if (lastdelim == 1)
						tmpstr += "" + bs[i];
				}
				i++;

			}while (i < bs.length);

			for(int q=0;q<2;q++)
			{
				i = 0;
				while (i < b.opCount)
				{
					if (operators[q].contains(b.peekOp(i)))
					{
						switch(b.popOp(i))
						{
							case "*" :
								tmp = Double.parseDouble(b.peekVal(i)) * Double.parseDouble(b.popVal(i+1));
								break;
								
							case "/" :
								tmp = Double.parseDouble(b.peekVal(i)) / Double.parseDouble(b.popVal(i+1));
								break;

							case "+" :
								tmp = Double.parseDouble(b.peekVal(i)) + Double.parseDouble(b.popVal(i+1));
								break;

							case "-" :
								tmp = Double.parseDouble(b.peekVal(i)) - Double.parseDouble(b.popVal(i+1));
								break;
						}
						b.putVal(Double.toString(tmp),i);
					}
					else
						i++;
				}
			}
            retval = Double.parseDouble(b.values[b.valCount-1]);
            return retval;
     }
        
    
    void run()
    {
        String s;
        
		while (true)
		{
            System.out.printf("\nInput expression : ");
            s = System.console().readLine()+"\n";

			if (s.toLowerCase().contains("q"))
				break;

			try
			{
				System.out.printf("Result: %.8f\n",reduce(s));
			}
			catch(Exception e)
			{
				System.out.printf("Error in expression!\n%s",e.getMessage());
			}
		}
    }
    
    public static void main(String[] args) {
        new jcalc().run();
    }
}