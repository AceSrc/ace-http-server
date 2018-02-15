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
  bool operator() (char * const &a, char * const &b) const {
    return strcmp(a, b) < 0;
  }
} ;

map<int, int> mp;

int main(int argc, char **argv) {
  auto a = mp.find(1);
  cout << (bool)(a == mp.end())  << endl;
  mp[1] = 2;
  a = mp.find(1);
  cout << a->first << ' ' << a->second << endl;
  a->second = 3;
  cout << mp[1] << endl;
}
