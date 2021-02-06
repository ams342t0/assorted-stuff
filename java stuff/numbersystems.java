import javax.swing.*;
import javax.swing.JOptionPane;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.filechooser.*;
import javax.swing.filechooser.FileFilter;
import java.awt.*;
import java.awt.event.*;
import java.text.*;
import java.util.*;
import java.io.*;
import java.io.File;
import java.util.regex.*;


class numbersystems
{
	JFrame  	 	windowframe;
	JPanel  	 	windowpanel;
	JButton 	 	buttonopen;
	JButton			buttonabout;
	JTable    	 	windowtable;
	JScrollPane  	scrollpane;
	JFileChooser 	textfilechooser;
	FileFilter   	filenamefilter;
	ActionListener  clickactionlistener;
	String[][]		tablecontents;
	String[]		
	columnheaders={"Decimal","Hexadecimal (H)","Octal (O)","Binary (B)"};
	FileReader	   	textfilereader;
	BufferedReader 	bufferedreader;
	File			selectedfile;
	ArrayList<String>	numberlist;
	String []		numberarray;
	TableModel 		tablemodel;
	Pattern  		regexpattern;
	Matcher  		regexmatcher;
	String			line;
	String			
	descline = "Description:\n" +
	  		   " numbersystems opens a text file containing a list of integers" +
				"as string literals expressed as one of the following:\n" +
				"	     - decimal     (contains digits 0-9)\n" +
				"	     - hexadecimal (contains digits 0-9, characters A-F and suffixed by 'h', ex. 0123ABCh)\n" +
				"	     - octal       (contains digits 0-7 and suffixed by 'o', ex. 0127o)\n" +
				"	     - binary      (contains either 0 or 1 and suffixed by 'b', ex 10011b)\n\n" +
				"Each line in the text file is read and checked if its a valid integer as defined above. \n" +
				"Any invalid integer	string format is ignored.\n\n" +
				"Use:\n" +
				"	Click OPEN SOURCE... to browse for a text file containing the list of integers.\n" +
				"	If the text file is read successfully all recognized valid integers" +
				" are displayed in decimal, hexadecimal, octal and binary.";


	public static void main(String[]s)
	{
		numbersystems ns = new numbersystems();
		ns.begin();		
	}
	
	public void doabout()
	{
		buttonabout = new JButton("ABOUT THIS PROJECT");
		buttonabout.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				JOptionPane.showMessageDialog(null,"NUMBER SYSTEMS\nBy\n...\n\n" + descline ,"About",JOptionPane.INFORMATION_MESSAGE);
			}
		});
	}
	
	public void begin()
	{
		buttonopen      = new JButton("OPEN SOURCE...");
		windowtable     = new JTable();
		windowframe     = new JFrame("Number System Conversion Table");
		scrollpane      = new JScrollPane(windowtable);
		filenamefilter  = new FileNameExtensionFilter("Source Text file","txt");
		textfilechooser = new JFileChooser();
		clickactionlistener = new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				int filechooseresult = textfilechooser.showOpenDialog(windowframe);
				selectedfile = textfilechooser.getSelectedFile();
				if (filechooseresult == JFileChooser.APPROVE_OPTION)
				{
					try
					{
						textfilereader = new FileReader(selectedfile.getPath());
						bufferedreader = new BufferedReader(textfilereader);
						numberlist  = new ArrayList<String>();
						try
						{
							while ((line=bufferedreader.readLine())!=null)
							{
								try
								{
									if (testnumstring(line,getnumbase(line)))
									numberlist.add(line);			
								}
								catch(Exception ix3){}
							}
						}
						catch(Exception ix2){}

						if (numberlist.size()>0)
						{
							numberarray = numberlist.toArray(new String[numberlist.size()]);
							tablecontents = new String[numberlist.size()][4];
							for(int ni=0;ni<numberarray.length;ni++)
							{
								tablecontents[ni][getnumbase(numberarray[ni])] = numberarray[ni].toUpperCase();
								try
								{
									switch (getnumbase(numberarray[ni]))
									{
										case 0:
										tablecontents[ni][1] = bintohex(dectobin(Integer.parseInt(numberarray[ni])));
										tablecontents[ni][2] = bintooct(dectobin(Integer.parseInt(numberarray[ni])));
										tablecontents[ni][3] = dectobin(Integer.parseInt(numberarray[ni]));
										break;

										case 1:
										tablecontents[ni][0] = ""+bintodec(hextobin(numberarray[ni]));
										tablecontents[ni][2] = bintooct(hextobin(numberarray[ni]));
										tablecontents[ni][3] = hextobin(numberarray[ni]);
										break;

										case 2:
										tablecontents[ni][0] = ""+bintodec(octtobin(numberarray[ni]));
										tablecontents[ni][1] = bintohex(octtobin(numberarray[ni]));
										tablecontents[ni][3] = octtobin(numberarray[ni]);
										break;

										case 3:
										tablecontents[ni][0] = ""+bintodec(numberarray[ni]);
										tablecontents[ni][1] = bintohex(numberarray[ni]);
										tablecontents[ni][2] = bintooct(numberarray[ni]);
										break;
												
									}
									}catch(Exception ixe){}
								}
						}
						else
						{
							tablecontents = new String[0][4];
							JOptionPane.showMessageDialog(null,"No valid string formatted integer found.","Empty",JOptionPane.INFORMATION_MESSAGE);
						}
						tablemodel  = new DefaultTableModel(tablecontents,columnheaders);
						windowtable.setModel(tablemodel);
						DefaultTableCellRenderer dtcr = new DefaultTableCellRenderer();
						dtcr.setHorizontalAlignment(JLabel.CENTER);
						windowtable.getColumnModel().getColumn(0).setCellRenderer(dtcr);
						windowtable.getColumnModel().getColumn(1).setCellRenderer(dtcr);
						windowtable.getColumnModel().getColumn(2).setCellRenderer(dtcr);
						windowtable.getColumnModel().getColumn(3).setCellRenderer(dtcr);
						bufferedreader.close();
					}
					catch(Exception ex)	{}
				}
			}
		};
		textfilechooser.setFileFilter(filenamefilter);
		buttonopen.addActionListener(clickactionlistener);
		windowframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		windowframe.setSize(800,600);
		doabout();
		windowframe.getContentPane().add(buttonopen,BorderLayout.PAGE_START);		
		windowframe.getContentPane().add(buttonabout,BorderLayout.PAGE_END);		
		windowframe.getContentPane().add(scrollpane,BorderLayout.CENTER);	
		windowframe.setVisible(true);
	}

	public boolean testnumstring(String s,int t)
	{
		String sp;

		if (s.length()==0) return false;

		switch (t)
		{
			case 1:
				sp = "^[0-9A-F]+[H]$";
				break;

			case 2:
				sp = "^[0-7]+[O]$";
				break;

			case 3:
				sp = "^[01]+[B]$";
				break;

			default:
				sp = "^[\\d]+$";
		}
		regexpattern = Pattern.compile(sp);
		regexmatcher = regexpattern.matcher(s.toUpperCase());
		return regexmatcher.find();
	}

	int getnumbase(String s)
	{
		int retval;

		String lastchar = s.substring(s.length()-1,s.length()).toUpperCase();

		switch(lastchar)
		{
			case "H":
				retval = 1;
				break;
				
			case "O":
				retval = 2;
				break;

			case "B":
				retval = 3;
				break;

			default :
				retval = 0;
		}
		return retval;
	}

	String flipstring(String s)
	{
		char []ps = s.toCharArray();
		char [] ts = new char[s.length()];

		for(int i=0;i<s.length();i++)
			ts[i] = ps[s.length()-i-1];		

		return new String(ts);
	}

	String bintohex(String bs)
	{
		char [] hs;
		int hexdigits;
		int bits,rembits;

		if (bs.length()==0) return "";

		bs = flipstring(bs).substring(1,bs.length());
		hexdigits = bs.length()/4;
		hs = new char[hexdigits+1];

		for(int i2=0;i2<=hexdigits;i2++) 
			hs[i2]='0';

		if (bs.length()%4!=0)
		{
			rembits = 4-(bs.length() - hexdigits*4);
			while (rembits>0)
			{
				bs+="0";
				rembits--;
			}
		}

		for(int ix=0,ti=0;ix<bs.length();ix+=4,ti++)
		{
			String gs = bs.substring(ix,ix+4);
			switch (bintodec(flipstring(gs)+"b"))
			{
				case 0: hs[ti]='0';break;
				case 1: hs[ti]='1';break;
				case 2: hs[ti]='2';break;
				case 3: hs[ti]='3';break;
				case 4: hs[ti]='4';break;
				case 5: hs[ti]='5';break;
				case 6: hs[ti]='6';break;
				case 7: hs[ti]='7';break;
				case 8: hs[ti]='8';break;
				case 9: hs[ti]='9';break;
				case 10: hs[ti]='A';break;
				case 11: hs[ti]='B';break;
				case 12: hs[ti]='C';break;
				case 13: hs[ti]='D';break;
				case 14: hs[ti]='E';break;
				case 15: hs[ti]='F';break;
			}
		}
		return flipstring(new String(hs))+"h";
	}

	String bintooct(String bs)
	{
		char [] hs;
		int hexdigits;
		int bits,rembits;

		if (bs.length()==0) return "";

		bs = flipstring(bs).substring(1,bs.length());
		hexdigits = bs.length()/3;
		hs = new char[hexdigits+1];

		for(int i2=0;i2<hexdigits+1;i2++) 
			hs[i2]='0';

		if (bs.length()%3!=0)
		{
			rembits = 3-(bs.length() - hexdigits*3);
			while (rembits>0)
			{
				bs+="0";
				rembits--;
			}
		}
		for(int ix=0,ti=0;ix<bs.length();ix+=3,ti++)
		{
			String gs = bs.substring(ix,ix+3);
			switch (bintodec(flipstring(gs)+"b"))
			{
				case 0: hs[ti]='0';break;
				case 1: hs[ti]='1';break;
				case 2: hs[ti]='2';break;
				case 3: hs[ti]='3';break;
				case 4: hs[ti]='4';break;
				case 5: hs[ti]='5';break;
				case 6: hs[ti]='6';break;
				case 7: hs[ti]='7';break;
			}
		}
		return flipstring(new String(hs))+"o";
	}

	int bintodec(String bs)
	{
		int rv=0,mask = 0x0000001;
		if (bs.length()==0) return 0;
		bs = flipstring(bs).substring(1,bs.length());
		char[]pi = bs.toCharArray();
		for(int i=0;i<bs.length();i++)
		{
			if (pi[i]=='1') 
				rv = rv | mask;
			mask = mask << 1;		
		}
		return rv;
	}

	String dectobin(int d)
	{
		char[] binstring;
		int i;

		binstring = new char[32];

		for (i=0;i<32;i++) binstring[i]='0';

		if (d < 0) return "-1";

		if (d == 0) return "0b";

		i = 0;

		do
		{
			if ((d & 0x00000001)==1) binstring[i]='1';
			else binstring[i] = '0';
			d = d >> 1;	
			i++;
		}
		while (d>0);
		return flipstring(new String(binstring).substring(0,i))+"b";		
	}
	
	String hextobin(String hs)
	{
		String os="";

		if (hs.length()==0) return "";

		hs = hs.substring(0,hs.length()-1).toUpperCase();
		char[]ps = hs.toCharArray();

		for(int i=0;i<hs.length();i++)
		{
			switch (ps[i])
			{
				case '0': os+="0000";break;
				case '1': os+="0001";break;
				case '2': os+="0010";break;
				case '3': os+="0011";break;
				case '4': os+="0100";break;
				case '5': os+="0101";break;
				case '6': os+="0110";break;
				case '7': os+="0111";break;
				case '8': os+="1000";break;
				case '9': os+="1001";break;
				case 'A': os+="1010";break;
				case 'B': os+="1011";break;
				case 'C': os+="1100";break;
				case 'D': os+="1101";break;
				case 'E': os+="1110";break;
				case 'F': os+="1111";break;
			}
		}
		return os+"b";
	}

	String octtobin(String o)
	{
		String os="";

		if (o.length()==0) return "";

		o = o.substring(0,o.length()-1);
		char[]ps = o.toCharArray();

		for(int i=0;i<o.length();i++)
		{
			switch (ps[i])
			{
				case '0': os+="000";break;
				case '1': os+="001";break;
				case '2': os+="010";break;
				case '3': os+="011";break;
				case '4': os+="100";break;
				case '5': os+="101";break;
				case '6': os+="110";break;
				case '7': os+="111";break;
			}
		}
		return os+"b";
	}
}