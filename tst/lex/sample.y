%{
#include <stdio.h>
void yyerror(const char *msg) { printf("Error Happend: %s\n", msg); }
%}

%union {
  int Val;
  char *msg;
};

%token<Val> TKNUM;
%type<Val> S;
%type<Val> E;

%left '+' '-'
%left '*' '/'

%%

S : S E '\n' { printf("ans = %d\n", $2); }
  | /* empty */ {}
  ;

E : E '+' E { $$ = $1 + $3; }
  | E '-' E { $$ = $1 - $3; }
  | E '*' E { $$ = $1 * $3; }
  | E '/' E { $$ = $1 / $3; }
  | TKNUM   { $$ = $1; }
  | '(' E ')' { $$ = $2; }

%%

int main() {
  return yyparse();
}
