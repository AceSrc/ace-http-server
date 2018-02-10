#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <code.h>
#include <sstream>
#include <map>


extern const char *DEFAULT_HTTP_VERSION;
extern const int DEFAULT_STATUS_CODE;
extern const char *DEFAULT_DESCRIPTION;
extern const char *HTTP_END_LINE;

class Response {
  typedef std::string key_type;
  typedef std::string value_type;

private:
  const char *http_version;
  int status_code;
  const char *description;

  std::string content;

  std::map<key_type, value_type> params;

  std::string get_localtime();
  std::string params_str();
  void update_params();

public:
  Response();

  char *c_str();
  std::string str();
  void update_content(const char *s);
} ;

#endif
