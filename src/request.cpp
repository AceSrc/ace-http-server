#include <request.h>
#include <stdio.h>

#include <iostream>
#include <cstring>
#include <cstdlib>

Request::Request() : action(NULL), path(NULL), version(NULL), content(NULL), buf(NULL), url_buf(NULL), speaker(NULL) {
}

Request::Request(Speaker *const speaker) : action(NULL), path(NULL), version(NULL), content(NULL), buf(NULL), url_buf(NULL), speaker(speaker) {
}

using namespace std;
static char *move_until(char *&p, const char *pattern) {
  char *rt = p;
  int size = strlen(pattern);
  if (size == 0) {p = nullptr; return rt;}
  while (p && *p) {
    //cout << p << ' ' << strncmp(p, pattern, size) << endl;
    if (strncmp(p, pattern, size) == 0) {
      *p = '\0';
      p += size;
      return strdup(rt);
    }
    ++p;
  }
  p = rt;
  return nullptr;
}

void Request::deurl(const char *s) {
  int size = strlen(s) + 1;
  if (!url_buf) url_buf = (char *)malloc(size);
  else url_buf = (char *)realloc(url_buf, size);
  memcpy(url_buf, s, size);
  char *p = url_buf;

  path = move_until(p, "?");
  if (!path) {
    path = strdup(s); 
    return ;
  }

  while (true) {
    char *key = move_until(p, "=");
    char *value = move_until(p, "&&");
    if (value == nullptr) {
      break;
    }
    url_params[key] = value;
  }
  free(url_buf);
}

void Request::destr() {
  action = speaker->split(" ");
  url = speaker->split(" ");
  version = speaker->split("\r");

  while (true) {
    if (speaker->test(1) == '\r') break;
    char *key = speaker->split(":", 1);
    char *val = speaker->split("\r", 1);
    params[key] = val;
  }

  if (params.count("Content-Length"))
    content = speaker->recv(atoi(params["Content-Length"])) + 3;
  deurl(url);
}

void Request::print() {
  printf("--------------------------------------\n");
  printf("%s\n%s\n%s\n\n", action, url, version);
  for (auto i = params.begin(); i != params.end(); i++) 
    printf("%s: %s\n", i->first, i->second);
  printf("**************************************\n");
  for (auto i = url_params.begin(); i != url_params.end(); i++) 
    printf("%s: %s\n", i->first, i->second);
  printf("**************************************\n");
  printf("\n%s\n", content);
  printf("--------------------------------------\n");
}

Request::~Request() {
  for (auto i = params.begin(); i != params.end(); i++) {
    if (i->first) free((void *)i->first);
    if (i->second) free((void *)i->second);
  }
  for (auto i = url_params.begin(); i != url_params.end(); i++) {
    if (i->first) free((void *)i->first);
    if (i->second) free((void *)i->second);
  }
}
