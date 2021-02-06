#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#define POLYs 25

char *term, *StrTerm;

void sounds()
{
	int i,snd;
	snd=225;
	for (i=1;i<=3;i++)
	{
	 sound(snd);
	 delay(10);
	 nosound();
	 delay(5);
	 nosound();
	 snd=snd+50;
	}
}

void sounds2()
{
	int i,snd;
	snd=525;
	for (i=1;i<=3;i++)
	{
		sound(snd);
		delay(10);
		nosound();
		delay(3);
    nosound();
    snd=snd+50;
	}
}

void sounds3()
{
	int i,snd;
	snd=150;
  for (i=1;i<=20;i++){
		sound(snd);
		delay(5);
    nosound();
    delay(5);
		nosound();
		snd=snd+75;
	}
}

void clear(int x1,int y1,int x2,int y2,int Col)
{
	int x,y;
	textbackground(Col);
	for (y=y1;y<=y2;y++)
		for (x=x1;x<=x2;x++)
    {
	 gotoxy(x,y);
	 putch('\0');
	}
}

void getint(int x,int y)
{
	 char count,xpos,key,use=0;
	 int good=0;
		 count=0;
		 xpos=x;
		 _setcursortype(_NORMALCURSOR);
	 while (good==0)
	 {
		 while(!kbhit())
		 {
			 gotoxy(xpos,y);
		 }
		 key=getch();
		 switch (key)
		 {
				case 13: sounds;
								 if (!isdigit(term[0]) || use > 1)
								 {
									 use=0;
									 xpos=x;
									 count=0;
									 gotoxy(xpos,y);
									 cputs("             ");
									 continue;
								 }
								 term[count]='\0';
								 good=1;
								 break;
				case 8 :	sounds2;
									if (count > 0) count--;
									if (xpos > x) xpos--;
									gotoxy(xpos,y);
									putch(' ');
									break;
				default: sounds3;
								 if(!isdigit(key) && key!='^') continue;
								 if (key=='^') use++;
								 term[count]=key;
								 gotoxy(xpos,y);
								 if (isdigit(key)) textcolor(10);
								 else if (key=='^') textcolor(LIGHTCYAN);
								 putch(key);
								 xpos++;
								 count++;
								 break;
		 }
		}
		return;
}

void _window(int x1,int y1,int x2,int y2,int FColor, int WColor)
{
	int x,y;
	textbackground(WColor);
	for (y=y1;y<=y2;y++)
		for (x=x1;x<=x2;x++)
    {
	 gotoxy(x,y);
	 putch('\0');
	}
  textcolor(FColor);
	for (x=x1;x<=x2;x++)
	{
		gotoxy(x,y1);
    putch(223);
		gotoxy(x,y2);
		putch(220);
	}
	for (y=y1;y<=y2;y++)
	{
		gotoxy(x1,y);
		putch(219);
		gotoxy(x2,y);
		putch(219);
	}

}

void textline(int x, int y, int FG, int BG, const char *Text)
{
	textcolor(FG);
	textbackground(BG);
	gotoxy(x,y);cputs(Text);
}

 typedef struct
	{
		long coef,deg,val;
	} Term;

 /* Procedures start here */

 void Setup_Poly (Term *Poly)
 {
	register itr;

	 if (!Poly->deg) Poly->val = 1;
		 else
			{
			 Poly->val = Poly->coef;
			 for (itr=2;itr <= Poly->deg;itr++ ) Poly->val*=Poly->coef;
		 }
 }

 int Pos(char *term)
 {
	for (register pos=0 ; (term[pos]!='^') && (term[pos]!=NULL) ;pos++)
		if (isalpha(term[pos]) ) return 0;
	if (pos==strlen(term)) pos = 0;
	return (pos);
	}

 int Read_Poly (Term *Poly,char *term)
 {
	char termstr[6];
	int Position = Pos(term);
	if (Position==0) return 0;
	strncpy(termstr,term,Position);
		Poly->coef =  atol(termstr);
	strcpy(termstr,term+Position+1);
		Poly->deg =  atol(termstr);
	return 1;
 }

 long Evaluate_Poly (Term Poly[],int nTerms,int Evaluate)
 {
	 long Sum = Poly[0].val,Difference = Poly[0].val;
	 register Traverse = 1;

	 if (Evaluate==2)
			for (;Traverse < nTerms;Traverse++) Sum += Poly[Traverse].val;
	 else
			for (;Traverse < nTerms;Traverse++) Difference -= Poly[Traverse].val;

	 _window(5,8,55,10,9,9);
	 _window(5,5,55,7,9,9);
	 int row,nCol;
   Traverse = 0;
	 textattr(14 + (9 << 4));
	 for (row=6; (Traverse<nTerms) ; Traverse++,row++)
	 {
		 gotoxy(19,row);
		 for (nCol=0; (nCol<5) && (Traverse<nTerms) ;nCol++,Traverse++)
		 {
				cprintf("%ld^%ld",Poly[Traverse].coef,Poly[Traverse].deg);
				if (Traverse < (nTerms-1) )
				if (Evaluate==2) cprintf(" + ");
				else cprintf(" - ");
		 }
	 }
	 textline(7,6,15,9,"Expression: ");
	 textline(7,9,0,9,(Evaluate==2)?"The sum of the polynomial term(s) is " :"The difference of the polynomial term(s) is ");
	 cprintf("%ld", (Evaluate==2) ? Sum : Difference);
	 getch();
	 clear(5,8,55,10,0);
	 clear(5,5,55,7,0);
	 return (Evaluate==2) ? Sum : Difference;
 }

void details()
{
	_window(8,7,68,19,4,2);
	textline(29,8,14,2,"*** POLYNOMIALS ***");
	textline(29,10,1,2,"PROJECT IN CEST 149");
	textline(15,12,15,2,"This program uses linked list to evaluate polynomial");
	textline(11,13,15,2, "expressions.");
	textline(11,15,4,2,"Submitted by: ");
	textline(40,15,4,2,"Submitted to: ");
	textline(42,16,15,2,"Engr. Elmer Maravillas");
	textline(16,16,15,2,"Armando M. So III");
	textline(17,17,14,2,"BS CpE-IV WMSU");
	getch();
	clear(8,7,68,19,0);
}

void selectbar()
{
	char x,key,index=0,xpos=5,select,intv=10;
	const char *texts[5]={"NEW TERM","ADDITION","SUBTRCT'N","DETAILS","EXIT"};
	Term Polynomial[POLYs];
	int Inputs = 0;

	clrscr();
	Polynomial[0].coef=Polynomial[0].deg=Polynomial[0].val=0;
	_setcursortype(_NOCURSOR);
	_window(xpos-2,1,xpos+47,3,4,2);
	textline(xpos,2,14,1,texts[0]);
	textline(xpos+intv,2,15,2,texts[1]);
	textline(xpos+intv*2,2,15,2,texts[2]);
	textline(xpos+intv*3,2,15,2,texts[3]);
	textline(xpos+intv*4,2,15,2,texts[4]);
	x=xpos;
	do{
	index=0;
	select=1;
	xpos=x;
	key=0;
	textline(xpos,2,14,1,texts[0]);
	while (key !=13)
	{
		while (1)
		{
			key=getch();
			if (key == 77 || key == 75 || key == 13) break;
		}
		textline(x,2,15,2,texts[0]);
		textline(x+intv,2,15,2,texts[1]);
		textline(x+intv*2,2,15,2,texts[2]);
		textline(x+intv*3,2,15,2,texts[3]);
		textline(x+intv*4,2,15,2,texts[4]);
		switch (key)
		{
			case 77:  sounds();
					if (index < 4) ++index; else if (index==4) index=0;
					if (xpos < (x+intv*4)) xpos+=intv;	else if (xpos == (x+intv*4)) xpos=x;
					if (select < 5) select++;else if (select ==5) select=1;
					textline(xpos,2,14,1,texts[index]);
					break;
			case 75:  sounds();
					if (index > 0) --index; else if (index==0) index=4;
					if (xpos > x) xpos-=intv; else if (xpos == x) xpos=x+intv*4;
					if (select >1) select--;else if (select ==1) select=5;
					textline(xpos,2,14,1,texts[index]);
					break;
			default: sounds2();
				 break;
	 }
		}
		switch (select)
		{
	 case 1:  _window(5,7,36,12,7,2);
						textline(7,8,14,2,"Syntax: M^N, M-base, N-power");
						textline(7,10,0,2,"INPUT: ");
						textline(7,11,0,2,"Value: ");
						_setcursortype(_NORMALCURSOR);
						getint(14,10);
						Read_Poly(&Polynomial[Inputs],term);
						Setup_Poly(&Polynomial[Inputs]);
						gotoxy(14,11);
						textcolor(4);
						textbackground(2);
						printf("%d",Polynomial[Inputs].val);
						getchar();
						clear(5,7,36,12,0);
						Inputs++;
						break;
	 case 2:
	 case 3:
						if (Inputs==0)
						{
							_window(5,7,28,9,1,2);
							textline(7,8,0,2,"NO TERMS TO EVALUATE.");
							delay(1200);
							clear(5,7,28,9,0);
							break;
						} else
						Evaluate_Poly(Polynomial,Inputs,select);
						break;
	 case 4: details();break;
	 case 5: break;
		}
 }while (select !=5);
}


 main ()
 {
	 selectbar();
	 _setcursortype(_NORMALCURSOR);
	 textbackground(0);
	 clrscr();
	 return 0;
}

