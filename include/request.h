#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <map>
#include <regex>
#include <string>
#include <type.h>

class Request {
private:
  char *action;
  char *path;
  char *url;
  char *version;

  params_type params;
  params_type url_params;

  char *content;

  char *buf;
  char *url_buf;
  void deurl(const char *s);
public:
  Request();
  void destr(const char *s);
  void print();
} ;

#endif
