#include "type.hpp"
#include <iostream>
#include <string.h>
extern int yyparse();
extern int yylex();
extern FILE *yyin;

params_type params;
std::string rt;
char *get_content(const std::string &filename, std::string query) {
  params.clear();
  std::cout << filename << ' ' << query << std::endl;
  rt = "";

  while (query.size() != 0) {
    auto i = query.find("&&");
    std::string cur;
    if (i != std::string::npos) {
      cur = query.substr(0, i);
      query = query.substr(i + 2);
    }
    else {
      cur = query;
      query = "";
    }
    i = cur.find("=");
    params[cur.substr(0, i)] = cur.substr(i + 1);
  }
  for (auto i = params.begin(); i != params.end(); i++)
    std::cout << i->first << ' ' << i->second << std::endl;

  yyin = fopen(filename.c_str(), "r");
  std::cout << yyin << std::endl;
  yyparse();
  std::cout << "Get Content" << std::endl;
  std::cout << rt << std::endl;
  std::cout << "------------------" << std::endl;
  return strdup(rt.c_str());
}
