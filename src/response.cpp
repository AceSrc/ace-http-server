#include <fastcgi.h>
#include <response.h>
#include <global.h>
#include <tools.h>


const char *DEFAULT_HTTP_VERSION = "HTTP/1.1";
const int DEFAULT_STATUS_CODE = SUCCESSFUL_RESPONSE;
const char *DEFAULT_DESCRIPTION = "OK";
const char *HTTP_END_LINE = "\r\n";

Response::Response(Request *request, Speaker *speaker) : http_version(DEFAULT_HTTP_VERSION), status_code(DEFAULT_STATUS_CODE), description(DEFAULT_DESCRIPTION), content(nullptr), request(request), speaker(speaker) {
}

static char *get_localtime() { 
  /* TODO: mutex */
  const int BUF_SIZE = 30;
  static char buf[BUF_SIZE];
  //Tue, 15 Nov 1994 08:12:31 GMT
  time_t t = time(0);
  strftime(buf, BUF_SIZE, "%a, %d %b %y %T GMT", localtime(&t));
  return buf;
}

void Response::update_param(const char *key, const char *val) {
  auto ptr = params.find(key);
  if (ptr == params.end()) {
    params[ bufdup(key) ] = bufdup(val);
  }
  else {
    free((void *)ptr->second);
    ptr->second = bufdup(val);
  }
}

void Response::update_param(const char *key, const int val) { 
  char buf[32];
  sprintf(buf, "%d", val);
  update_param(key, buf);
}

#include <iostream>
using namespace std;
void Response::get_fastcgi_response(const char *suffix) {
  Fastcgi fastcgi("127.0.0.1", atoi(GLOBALS[suffix])); 
  //cout << suffix << ' ' << atoi(GLOBALS[suffix])  << endl;

  params_type params;
  Buffer buffer;
  buffer << GLOBALS["root"] << request->path;
  params["SCRIPT_FILENAME"] = buffer.str();
  params["QUERY_STRING"] = "";
  params["REQUEST_METHOD"] = "GET";
  fastcgi.send(0x0001, params, "");
  fastcgi.recv();
  char *p = fastcgi.get_stdout();
  while (!(*p = '\r' && *(p + 1) == '\n' && *(p + 2) == '\r' && *(p + 3) == '\n')) p++;
  p += 4;
  //printf("-----------\n%s\n", p);
  content = bufdup(p);
  update_param("Content-type", "text/html; charset=UTF-8");
  //cout << content << endl;
  update_param("Content-Length", strlen(content));
}

void Response::get_resource() {
  char *suffix = nullptr;
  for (int i = strlen(request->path) - 1; i >= 0; i--) {
    if (request->path[i] == '.') {
      suffix = request->path + i + 1;
      break;
    }
  }
  if (suffix && GLOBALS[suffix]) {
    get_fastcgi_response(suffix);
    return ;
  }

  const int BUF_SIZE = 655360;
  char buf[BUF_SIZE];

  sprintf(buf, "%s%s", GLOBALS["root"], request->path);
  if (content) free(content);
  content = nullptr;

  auto fd = fopen(buf, "r");
  if (fd) {
    content_size = fread(buf, 1, BUF_SIZE, fd);
    content = (char *)malloc(content_size);
    memcpy(content, buf, content_size);
    sprintf(buf, "%d", content_size);
    update_param("Content-Length", buf);
    fclose(fd);
  }
}

void Response::update_response() {
  update_param("Date", get_localtime());

  get_resource();
  if (!content) {
    content = (char *)malloc(1);
    *content = '\0';
    update_param("Content-Length", "0");
  }
  //cout << content << ' ' << params["Content-Length"] << endl;
}

void Response::send_resource() {
  update_response();

  const int BUF_SIZE = 65536;
  char *buf = (char *)malloc(BUF_SIZE);

  sprintf(buf, "%s %d %s\r\n", http_version, status_code, description);
  speaker->send(buf);

  for (auto i = params.begin(); i != params.end(); i++) {
    sprintf(buf, "%s: %s\r\n", i->first, i->second);
    speaker->send(buf);
  }

  speaker->send("\r\n");
  //cout << content << ' ' << contetn_size << endl;
  speaker->send(content, atoi(params["Content-Length"]));
  free(buf);
}

//std::string Response::params_str() {
  //stringstream ss;
  //for (auto i = params.begin(); i != params.end(); ++i) {
    //ss << (*i).first << ": " << (*i).second << HTTP_END_LINE;
  //}
  //return ss.str();
//}

//void Response::update_params() {
  //stringstream ss;
  //ss << content.size();
  //params["Content-Length"] = ss.str();
  //ss.clear();

  //params["Date"] = this->get_localtime();
//}

//void Response::update_content(const char *s) {
  //content = string(s);
//}
//


