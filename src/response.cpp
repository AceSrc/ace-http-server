#include <fastcgi.h>
#include <response.h>
#include <global.h>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>


const std::string DEFAULT_HTTP_VERSION = "HTTP/1.1";
const int DEFAULT_STATUS_CODE = 200;
const std::string DEFAULT_DESCRIPTION = "OK";
const std::string HTTP_END_LINE = "\r\n";

Response::Response(Request *request, Speaker *speaker, Logger *logger) : http_version(DEFAULT_HTTP_VERSION), status_code(DEFAULT_STATUS_CODE), description(DEFAULT_DESCRIPTION), content(""), request(request), speaker(speaker), logger(logger) {
}

static std::string get_localtime() { 
  const int BUF_SIZE = 30;
  char buf[BUF_SIZE];
  //Tue, 15 Nov 1994 08:12:31 GMT
  time_t t = time(0);
  strftime(buf, BUF_SIZE, "%a, %d %b %y %T GMT", localtime(&t));
  return std::string(buf);
}

void Response::get_fastcgi_response(std::string suffix) {
  Fastcgi fastcgi("127.0.0.1", std::stoi(GLOBALS[suffix])); 

  if (!fastcgi.ok()) {
    logger->format("Can't connect to Fastcgi server(%s)", suffix.c_str());
    request->path = GLOBALS["error_page"];
    send_resource();
    return ;
  }

  params_type fastcgi_params;

  fastcgi_params["SCRIPT_FILENAME"] = GLOBALS["root"] + request->path;
  fastcgi_params["QUERY_STRING"] = request->query_string;
  fastcgi_params["REQUEST_METHOD"] = request->request_method;
  fastcgi_params["CONTENT_TYPE"] = "";
  fastcgi_params["CONTENT_LENGTH"] = "";
  fastcgi_params["SCRIPT_NAME"] = request->path;
  fastcgi_params["REQUEST_URI"] = request->path;
  fastcgi_params["DOCUMENT_URI"] = request->path;
  fastcgi_params["DOCUMENT_ROOT"] = GLOBALS["root"];
  fastcgi_params["SERVER_PROTOCOL"] = "HTTP/1.1";
  fastcgi_params["REQUEST_SCHEME"] = "http";
  fastcgi_params["GATEWAY_INTERFACE"] = "CGI/1.1";
  fastcgi_params["REDIRECT_STATUS"] = "200";
  fastcgi_params["SERVER_NAME"] = "localhost";
  fastcgi_params["SERVER_ADDR"] = "127.0.0.1";
  fastcgi_params["SERVER_PORT"] = "8080";
  fastcgi_params["REMOTE_ADDR"] = logger->ip;
  fastcgi_params["REMOTE_PORT"] = std::to_string(logger->port);
  fastcgi_params["SERVER_SOFTWARE"] = "ace-http";
   
  std::string cur;
  for (auto i = request->params.begin(); i != request->params.end(); i++) {
    cur = i->first; 
    for (auto &x : cur) x = std::toupper(x);
    //std::transform(cur.begin(), cur.end(), cur.begin(), std::toupper);
    std::replace(cur.begin(), cur.end(), '-', '_');
    //std::cout << i->first << ' ' << i->first.size() << std::endl;
    //if (i->first == "Cookie") std::cout << "H" << std::endl; 
    fastcgi_params["HTTP_" + cur] = i->second;
  }

  fastcgi.send(0x0001, fastcgi_params, "");
  fastcgi.recv();
  content = fastcgi.get_stdout();
  //std::cout << fastcgi.get_stderr() << std::endl;
  auto i = content.find("\r\n\r\n");
  if (i != std::string::npos) {
    header_append = content.substr(0, i + 2);
    content = content.substr(i + 4);
  }
  else header_append = "";
  params["Content-type"] = "text/html; charset=UTF-8";
  params["Content-Length"] = std::to_string(content.size());
  logger->format("Fastcgi(%s) Content-Length: %d", suffix.c_str(), content.size());
}

void Response::get_resource() {
  logger->format("Asking for %s", request->path.c_str());
  std::string suffix = "";
  {
    std::string::size_type suffix_pos = request->path.rfind(".");
    if (suffix_pos != std::string::npos) {
      suffix = request->path.substr(suffix_pos + 1);
      if (GLOBALS.count(suffix)) {
        get_fastcgi_response(std::move(suffix));
        return ;
      }
    }
  }

  header_append = "";
  std::ifstream fin(GLOBALS["root"] + request->path);
  if (fin.is_open()) {
    std::stringstream ss;
    ss << fin.rdbuf();
    content = ss.str();
  }
  else {
    logger->format("Failed to open file %s", (GLOBALS["root"] + request->path).c_str());
    request->path = GLOBALS["error_page"];
    get_resource();
    return ;
  }
  if (GLOBALS.count(suffix + "-type")) params["Content-type"] = GLOBALS[suffix + "-type"];
  else params["Content-type"] = "text/html";
  params["Content-Length"] = std::to_string(content.size());
  logger->format("Text(%s) Content-Length: %d(%.2fK)", params["Content-type"].c_str(), content.size(), content.size() / 8.0 / 1024.0);
}

void Response::update_response() {
  params["Date"] = get_localtime();
  get_resource();
}

void Response::send_resource() {
  update_response();
  std::stringstream ss;

  ss << http_version << ' ' <<
        status_code << ' ' <<
        description << "\r\n";

  for (auto i = params.begin(); i != params.end(); i++) {
    ss << i->first << ": " << i->second << "\r\n";
  }
  //std::cout << content << std::endl;
  ss << header_append;
  ss << "\r\n";
  ss << content;
  //std::cout << ss.str() << std::endl;
  speaker->send(ss.str());
}
