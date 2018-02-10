#include <request.h>
#include <stdio.h>

#include <iostream>
#include <cstring>
#include <cstdlib>

Request::Request() : action(NULL), path(NULL), version(NULL), content(NULL), buf(NULL), url_buf(NULL) {
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
      return rt;
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
  if (!path) return ;

  while (true) {
    char *key = move_until(p, "=");
    char *value = move_until(p, "&&");
    if (value == nullptr) {
      value = p;
      url_params[key] = value;
      break;
    }
    url_params[key] = value;
  }
}

void Request::destr(const char *s) {
  int size = strlen(s) + 1;
  if (!buf) buf = (char *)malloc(size);
  else buf = (char *)realloc(buf, size);
  memcpy(buf, s, size);
  char *p = buf;

  action = move_until(p, " ");
  url = move_until(p, " ");
  version = move_until(p, "\r\n");

  while (true) {
    if (p[0] == '\r' && p[1] == '\n') {
      p += 2;
      break;
    }
    char *key = nullptr, *value = nullptr;
    key = move_until(p, ": ");
    if (key == nullptr) break;
    value = move_until(p, "\r\n");
    params[key] = value;
  }
  content = move_until(p, "");

  deurl(url);
}

void Request::print() {
  printf("--------------------------------------\n");
  printf("%s\n%s\n%s\n\n", action, path, version);
  for (auto i = params.begin(); i != params.end(); i++) 
    printf("%s: %s\n", i->first, i->second);
  printf("**************************************\n");
  for (auto i = url_params.begin(); i != url_params.end(); i++) 
    printf("%s: %s\n", i->first, i->second);
  printf("**************************************\n");
  printf("\n%s\n", content);
  printf("--------------------------------------\n");
}
