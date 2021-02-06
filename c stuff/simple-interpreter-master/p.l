%{
#include <stdio.h>
#include <stdlib.h>
#include "p.tab.h"
%}

%option noyywrap
%%
help					{return HELP;}
exit|quit				{exit(0);}
[0-9]*(\.[0-9]*)?		{yylval.f = (double)atof(yytext);return NUM_CONST;}
del|DEL					{return DELVAR;}
count|COUNT				{return COUNTCMD;}
to|TO|\.\.				{return TO;}
[-+*/()=\n]				{return *yytext;}
\".*\"					{*(yytext+yyleng-1)=0;sprintf(yylval.s,"%s",yytext+1);return STR_CONST;}
print|PRINT				{return PRINTCMD;}
[a-zA-Z_][a-zA-Z0-9_]*	{sprintf(yylval.s,"%s",yytext);return VARNAME;}
\s\t\r\f				{;}
%%

