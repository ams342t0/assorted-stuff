%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR 1023
typedef struct s_var
{
        char name[64];
        char value[256];
}variable;

variable variables[MAX_VAR];
long     var_count =-1;
FILE *yyin,*yyout;

char *var_value(char *);
int new_variable(char *,char *);
void del_var(char *);
extern int yyparse();
void yyerror(char *);
void prompt();
void show_help();
%}

%union
{
	char   s[256];
	double f;
}

%left '+' '-'
%left '*' '/'
%left '(' ')'
%left '='

%token <s> STR_CONST VARNAME COUNTCMD TO PRINTCMD  EXITCMD DELVAR HELP
%token <f> NUM_CONST
%type  <s> STATEMENT ASSIGN COUNT PRINT
%type  <f> NUM_EXP TERM FACTOR

%%
STATEMENTS	:	STATEMENTS STATEMENT '\n'
			|   
			;
			
STATEMENT	:	NUM_EXP					{fprintf(yyout,"%.8f",$1);prompt();}
			|	PRINT					{prompt();}	
			|   COUNT					{prompt();}
			|	ASSIGN					{prompt();}
			|	EXITCMD
			|   DELVAR VARNAME			{del_var($2);prompt();}
			|	HELP					{show_help();prompt();}
			|	error					{yyerror("Syntax error!");prompt();}
			;
		
NUM_EXP		:	NUM_EXP '+' FACTOR		{$$ = $1 + $3;}
			|	NUM_EXP '-' FACTOR		{$$ = $1 - $3;}
			|	FACTOR					{$$ = $1;}
			;
			
FACTOR		:	FACTOR '*' TERM			{$$ = $1 * $3;}
			|	FACTOR '/' TERM			{
											if ($3 == 0.0f)
											{
												$$ = 0.0;
												fprintf(yyout,"Can't divide by zero!");
												prompt();
											}
											else
												$$ = $1 / $3;
										}
			| 	TERM					{$$ = $1;}
			;
			
TERM		:	'(' NUM_EXP ')'			{$$ = $2;}
			|	NUM_CONST				{$$ = $1;}
			|	VARNAME					{$$ = (double)atof(var_value($1));}
			;

COUNT		:	COUNTCMD NUM_EXP TO NUM_EXP 	{
													long i, a, b,ctr;
													a = (long)$2;
													b = (long)$4;
													if (a < b)
														i = 1;
													else
														i = -1;
													for(ctr=a;ctr != b;ctr+=i)
													{
														fprintf(yyout,"%d",ctr);
														if (ctr != $4) fprintf(yyout,", ");
													}
													fprintf(yyout,"%d",ctr);
												}
			;
			
ASSIGN		:	VARNAME '=' NUM_EXP	{
										char tmp_str[256];
										sprintf(tmp_str,"%.8f",$3);
										if (new_variable($1,tmp_str))
											fprintf(yyout,"\rVariable %s created.",variables[var_count].name);
									}
			|	VARNAME '=' STR_CONST	{
											if (new_variable($1,$3))
												fprintf(yyout,"\rVariable %s created.",variables[var_count].name);
										}

			|	VARNAME '=' VARNAME {
										if (new_variable($1,var_value($3)))
											fprintf(yyout,"\rVariable %s created.",variables[var_count].name);
									}
			;			
						
PRINT		:	PRINTCMD STR_CONST		{fprintf(yyout,"\r%s",$2);}
			|   PRINTCMD VARNAME		{
											char *s = var_value($2);
											if (s)
												fprintf(yyout,"\r%s",var_value($2));
											else
												fprintf(yyout,"\r%s does not exist.",$2);
										}
			;	
	
%%

int main(int argc, char **argv)
{
	FILE *f;
	yyout = stdout;
	if (argc == 0)
		yyin = stdin;
	else
	{
		f = fopen(argv[1],"r");
		if (f)
			yyin = f;
	}
	printf("A GNU PATAKA INTERPRETER\n\n");
	prompt();
	yyparse();
	if (f)
		fclose(f);
    return 1;
}

void yyerror(char *m)
{
	fprintf(yyout,"\n%s",m);
}

char *var_value(char *name)
{
	int ctr;
    for(ctr=0;ctr<=var_count;ctr++)
         if (strcmp(variables[ctr].name,name) == 0)
            return variables[ctr].value;
    return 0;
}

int new_variable(char *name,char *value)
{
	int ctr;
	
	if (var_count == MAX_VAR)
	{
		fprintf(yyout,"\rCannot create new variable.");
	 return 0;
	}

	for(ctr =0;ctr<= var_count;ctr++)
        if (strcmp(variables[ctr].name,name) == 0 || variables[ctr].name[0]==0)
		{
			strcpy(variables[ctr].name,name);
			strcpy(variables[ctr].value,value);
			fprintf(yyout,"\r%s reassigned as %s",name,value);
            return 0;
		}
		
     var_count++;
     strcpy(variables[var_count].name,name);
     strcpy(variables[var_count].value,value);
	 return 1;
} 

void del_var(char *name)
{
	int ctr;
    for(ctr=0;ctr<=var_count;ctr++)
         if (strcmp(variables[ctr].name,name) == 0)
         {
			fprintf(yyout,"\rVariable %s removed.",name);
            variables[ctr].name[0] = 0;
            return;
         }
	fprintf(yyout,"\r%s does not exist.",name);
}


void prompt()
{
	fprintf(yyout,"\n  << ");
}

void show_help()
{
	fprintf(yyout,"\nPRINT <exp>   - Print; <exp> is string or variable.\n"
	        "COUNT <from> TO <to>  - Print values between range.\n"
			"<varname> = <exp>     - Assign <exp> to <varname>, <exp> is arithmetic, string or variable.\n"
			"DEL <variable>        - Delete variable\n"
			"EXIT|QUIT             - Exit to command prompt\n");
}