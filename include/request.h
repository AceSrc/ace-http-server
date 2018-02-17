#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <string>
#include <speaker.h>
#include <type.h>

class Request {
public:
  std::string query_string;
  std::string request_method;

  std::string path;
  std::string url;
  std::string version;

  params_type params;
  params_type url_params;

  //params_type GLOBALS;
  std::string content;

  std::string buf;
  std::string url_buf;
  void deurl(const std::string &s);

  Speaker *speaker;

  Request(Speaker * const speaker = nullptr);
  ~Request();
  void destr();
  void print();
} ;

#endif

