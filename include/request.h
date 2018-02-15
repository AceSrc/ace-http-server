#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <string>
#include <speaker.h>
#include <type.h>

class Request {
public:
  char *action;
  char *path;
  char *url;
  char *version;

  params_type params;
  params_type url_params;

  //params_type GLOBALS;
  char *content;

  char *buf;
  char *url_buf;
  void deurl(const char *s);

  Speaker *speaker;

  Request();
  Request(Speaker * const speaker);
  ~Request();
  void destr();
  void print();
} ;

#endif

