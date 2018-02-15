#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <code.h>
#include <sstream>
#include <map>
#include <type.h>
#include <buffer.h>
#include <request.h>
#include <speaker.h>


extern const char *DEFAULT_HTTP_VERSION;
extern const int DEFAULT_STATUS_CODE;
extern const char *DEFAULT_DESCRIPTION;
extern const char *HTTP_END_LINE;

class Response {
private:
  const char *http_version;
  int status_code;
  const char *description;

  //char *header;
  char *content;
  params_type params;
  Request *request;
  Speaker *speaker;

  void update_response();
  void get_fastcgi_response(const char *suffix);
  void get_resource();
  int content_size;

  void update_param(const char *key, const char *val);
  void update_param(const char *key, const int val);
public:
  Response(Request *request = nullptr, Speaker *speaker = nullptr);
  void send_resource();
} ;

#endif

