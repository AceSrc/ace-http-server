#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <cstring>
#include <cstdio>
#include <cstdlib>

const int INT_MAX_LEN = 32;
class atoi {
private:
  char s[INT_MAX_LEN];
public:
  atoi(int x) {
    sprintf(s, "%d", x);
  }
  char *str() {
    return s;
  }
  ~atoi();
} ;

char *bufdup(const char *s, int size = -1);
#endif
