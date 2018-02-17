%{
#include <cstdio>
#include <iostream>
#include <string>
#include "type.hpp"

extern int yylex(); 
extern params_type params;

extern std::string rt;
void yyerror(const char *s) {
  printf("ERROR: %s\n", s);
  exit(1);
}
%}

%union {
  std::string *str;
  int token;
  int int_value;
} ;

%token <str> TK_WORD
%token <int_value> TK_INT
%token <str> TK_OTHER

%type <str> program
%type <str> stats
%type <str> stat

%type <int_value> calc
%type <int_value> operand

%type <str> variable

%start program

%%

program : stats
        ;

stats : stat
      | stats stat
      ;

stat : '`' calc '`' { rt += std::to_string($2); }
     | variable { rt += params[*$1]; }
     | TK_WORD { rt += *$1; }
     | TK_OTHER { rt += *$1; }
     | TK_INT { rt += std::to_string($1); }
     | '+' { rt += '+'; }
     | '-' { rt += '-'; }
     | '*' { rt += '*'; }
     | '/' { rt += '/'; }
     | '$' { rt += '$'; }
     | '(' { rt += '('; }
     | ')' { rt += ')'; }
     ;

calc : operand
     | '(' calc ')' { $$ = $2; }
     | calc '+' operand { $$ = $1 + $3; }
     | calc '-' operand { $$ = $1 - $3; }
     | calc '*' operand { $$ = $1 * $3; }
     | calc '/' operand { $$ = $1 / $3; }

operand : TK_INT { $$ = $1; }
        | variable { $$ = stoi(params[*$1]); };
        ;

variable : '$' TK_WORD { $$ = $2; }
         ;
