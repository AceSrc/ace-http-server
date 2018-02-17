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
#include <unordered_map>
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

unordered_map<string, string> mp;
int main(int argc, char **argv) {
  mp["HelloWorld"] = "Kelly";
  mp["Content-Length"] = "123";
  for (auto i = mp.begin(); i != mp.end(); i++) 
    cout << i->first << ' ' << i->second << endl;
  return 0;
}
