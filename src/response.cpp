#include <response.h>
using namespace std;


const char *DEFAULT_HTTP_VERSION = "HTTP/1.1";
const int DEFAULT_STATUS_CODE = SUCCESSFUL_RESPONSE;
const char *DEFAULT_DESCRIPTION = "OK";
const char *HTTP_END_LINE = "\r\n";

Response::Response() : http_version(DEFAULT_HTTP_VERSION), status_code(DEFAULT_STATUS_CODE), description(DEFAULT_DESCRIPTION) {
}

char *Response::c_str() {
  return nullptr;
  //return this->str().c_str();
}

std::string Response::str() {
  stringstream ss;
  update_params();

  ss << http_version << ' ' << status_code << ' ' << description << HTTP_END_LINE;
  ss << params_str();
  ss << HTTP_END_LINE;
  ss << content;
  return ss.str(); 
}

std::string Response::get_localtime() {
  const int BUFLEN = 32;
  char buf[BUFLEN];
  //Tue, 15 Nov 1994 08:12:31 GMT
  time_t t = time(0);
  strftime(buf, BUFLEN, "%a, %d %b %y %T GMT", localtime(&t));
  return std::string(buf);
}

std::string Response::params_str() {
  stringstream ss;
  for (auto i = params.begin(); i != params.end(); ++i) {
    ss << (*i).first << ": " << (*i).second << HTTP_END_LINE;
  }
  return ss.str();
}

void Response::update_params() {
  stringstream ss;
  ss << content.size();
  params["Content-Length"] = ss.str();
  ss.clear();

  params["Date"] = this->get_localtime();
}

void Response::update_content(const char *s) {
  content = string(s);
}
