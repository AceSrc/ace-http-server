#include <fastcgi.h>
#include <response.h>
#include <global.h>
#include <string>
#include <sstream>
#include <fstream>


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

  params_type fastcgi_params;
  fastcgi_params["SCRIPT_FILENAME"] = GLOBALS["root"] + request->path;
  fastcgi_params["QUERY_STRING"] = request->query_string;
  fastcgi_params["REQUEST_METHOD"] = request->request_method;
  fastcgi.send(0x0001, fastcgi_params, "");
  fastcgi.recv();
  content = fastcgi.get_stdout();
  auto i = content.find("\r\n\r\n");
  if (i != std::string::npos)
    content = content.substr(content.find("\r\n\r\n") + 4);
  params["Content-type"] = "text/html; charset=UTF-8";
  params["Content-Length"] = std::to_string(content.size());
  logger->format("Fastcgi(%s) Content-Length: %d", suffix.c_str(), content.size());
}

void Response::get_resource() {
  logger->format("Asking for %s", request->path.c_str());
  std::string suffix = "";
  {
    std::string::size_type suffix_pos = request->path.rfind(".");
    //std::cout << request->path << std::endl;
    if (suffix_pos != std::string::npos) {
      suffix = request->path.substr(suffix_pos + 1);
      if (GLOBALS.count(suffix)) {
        get_fastcgi_response(std::move(suffix));
        return ;
      }
    }
  }

  std::ifstream fin(GLOBALS["root"] + request->path);
  if (fin.is_open()) {
    std::stringstream ss;
    ss << fin.rdbuf();
    content = ss.str();
  }
  else {
    content = "";
    logger->format("Failed to open file %s", (GLOBALS["root"] + request->path).c_str());
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
  ss << "\r\n";
  ss << content;
  speaker->send(ss.str());
}
