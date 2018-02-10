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

int main(int argc, char **argv) {
  char a[100];  
  a[0] = 'a';
  a[1] = 'b';
  a[2] = 'c';
  a[3] = 'd';
  a[4] = 'e';
  cout << strtok(a, "bc") << ' ' << strtok(NULL, "e") << endl;
  return 0;
}