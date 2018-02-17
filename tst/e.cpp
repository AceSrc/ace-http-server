#include <sstream>
#include <fstream>
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

class A {
public:
  ~A() {
    cout << "Des A" << endl;
  }
} ;

class B {
  A a;
public:
  ~B() {
    cout << "Des B" << endl;
  }
} ;

int main(int argc, char **argv) {
  ifstream fin("input");
  stringstream ss;
  ss << fin.rdbuf();
  cout << ss.str().size() << endl;
  return 0;
}
