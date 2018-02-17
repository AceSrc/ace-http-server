#include <request.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <global.h>

Request::Request(Speaker *const speaker) : speaker(speaker){
}

void Request::deurl(const std::string &s) {
  int cur = 0, nxt = 0;
  nxt = s.find("?", cur);
  if (nxt == std::string::npos) {
    if (s.find(".") == std::string::npos) path = s + GLOBALS["index"];
    else path = s;
    query_string = "";
    return ;
  }
  cur = nxt + 1;
  path = s.substr(0, nxt);
  if (path.find(".") == std::string::npos) path += GLOBALS["index"];
  query_string = s.substr(cur);

  std::string key, val;
  while (true) {
    nxt = s.find("=", cur);
    key = s.substr(cur, nxt - cur + 1);
    cur = nxt + 1;

    nxt = s.find("&&", cur);
    if (nxt == std::string::npos) {
      val = s.substr(cur);
      url_params[key] = val;
      break;
    }
    val = s.substr(cur, nxt - cur + 1);
    url_params[key] = val;
    cur = nxt + 2;
  }
}

void Request::destr() {
  //std::cout << "destr" << std::endl;
  request_method = speaker->split(" ");
  url = speaker->split(" ");
  version = speaker->split("\r");

  while (true) {
    if (speaker->test(1) == '\r') break;
    std::string key = speaker->split(":", 1);
    std::string val = speaker->split("\r", 1);
    params[key] = val;
  }

  if (params.count("Content-Length"))
    content = speaker->recv(std::stoi(params["Content-Length"])).substr(3);
  deurl(url);
}

void Request::print() {
  using namespace std;
  printf("--------------------------------------\n");
  cout << request_method << endl << url << endl << version << endl << endl;
  for (auto i = params.begin(); i != params.end(); i++) 
    cout << i->first << ' ' << i->second << endl;
  printf("**************************************\n");
  for (auto i = url_params.begin(); i != url_params.end(); i++) 
    cout << i->first << ' ' << i->second << endl;
  printf("**************************************\n");
  cout << endl << content << endl;
  printf("--------------------------------------\n");
}

Request::~Request() {
}
