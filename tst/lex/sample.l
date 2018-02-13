%{
#include "sample.tab.h"
%}

DIGIT [0-9]
ID [a-z][a-z0-9]*

%%

{DIGIT}+ { yylval = atoi(yytext); return TKNUM; }

[-/+*()\n] { return yytext[0]; }

[ \t]* {}

. { return 0; }
%%

int yywrap(void) {
  return 1;
}
