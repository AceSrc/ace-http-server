%{
#include <string>
#include <iostream>
#include "parser.hpp"
%}

DIGIT [0-9]
WORD [a-zA-Z][a-zA-Z0-9]*

%%

{DIGIT}+ { 
  /*std::cout << yytext << std::endl; */
  yylval.int_value = atoi(yytext); 
  return TK_INT; 
  }
{WORD} {
  /*std::cout << yytext << std::endl; */
  yylval.str = new std::string(yytext, yyleng); 
  return TK_WORD;
  }
[()-/+*$`] {
  /*std::cout << (int)yytext[0] << '(' << yytext[0] << ')' << std::endl; */
  return yytext[0];
  }
. {
  /*std::cout << (int)yytext[0] << '(' << yytext[0] << ')' << std::endl; */
  yylval.str = new std::string(yytext, yyleng); 
  return TK_OTHER; 
  }

%%

int yywrap() {
  return 1;
}
