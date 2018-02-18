#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <sstream>
#include <type.h>
#include <request.h>
#include <speaker.h>
#include <logger.h>


extern const std::string DEFAULT_HTTP_VERSION;
extern const int DEFAULT_STATUS_CODE;
extern const std::string DEFAULT_DESCRIPTION;
extern const std::string HTTP_END_LINE;

class Response {
private:
  std::string http_version;
  int status_code;
  std::string description;

  //char *header;
  std::string header_append;
  std::string content;
  params_type params;
  Request *request;
  Speaker *speaker;
  Logger *logger;

  void update_response();
  void get_fastcgi_response(std::string suffix);
  void get_resource();
  int content_size;

  //void update_param(const char *key, const char *val);
  //void update_param(const char *key, const int val);
public:
  Response(Request *request = nullptr, Speaker *speaker = nullptr, Logger *logger = nullptr);
  void send_resource();
} ;

#endif

