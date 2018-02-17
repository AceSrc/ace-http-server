#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;
#ifndef __DEBUG__
  #define debug NULL
#else
  #define debug printf("%s %d\n", __FUNCTION__, __LINE__);
#endif
#define Assert(cond, ...) \
  do { \
    if (!cond) { \
      printf("\33[4;31mLINE NUMBER:\33[0m %d \n", __LINE__); \
      printf(__VA_ARGS__); \
      assert(cond); \
    } \
  } while (0)
const int inf = 2147483647;

class T {
public:
  int a;
  char *p;
  T(int value = 10) {
    a = value;
    p = (char *)malloc(120);
    p = "Hello";
    cout << "Empty construct" << endl;
  }

  T(T &&x) = default;
  //{
    //cout << "Move construct" << endl;
  //}

  T &operator=(T &&x) = default;
  T(const T&x) {
    cout << "Copy construct" << endl;
  }

  ~T() {
    free(p);
    cout << "Destruct" << endl;
  }
} ;

T f(T a) {
  //cout << a.p << endl;
  T rt;
  return rt;
}

int main(int argc, char **argv) {
  string s = "Hello";
  cout << s.substr(string::npos) << endl;
}

