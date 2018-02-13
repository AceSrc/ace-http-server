#include <functional>
#include <memory>
#include <stdlib.h>
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
  T() {
    cout << "Construct" << endl;
  }
  ~T() {
    cout << "Destroyed" << endl;
  }
} ;

typedef unique_ptr<T> uptr;

int main(int argc, char **argv) {
  queue<uptr> q;
  q.push(uptr(new T));

  uptr a = std::move(q.front());
  uptr b = std::move(q.front());
  if (!b) cout << "!b" << endl;
  if (!a) cout << "!a" << endl;
  q.pop();
  return 0;
}
