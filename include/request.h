#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <map>
#include <regex>
#include <string>

class Request {
  typedef char * key_type;
  typedef char * value_type;
private:
  char *action;
  char *path;
  char *url;
  char *version;

  std::map<key_type, value_type> params;
  std::map<key_type, value_type> url_params;

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
