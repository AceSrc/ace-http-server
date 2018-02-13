#include <chrono>
#include <thread>
#include "message_queue.h"
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
    //cout << "Build" << endl;
  }
  ~T() {
    //cout << "Delete" << endl;
  }
  int id;
} ;
typedef unique_ptr<T> uptr;

Queue<T> q;
void ok() {
  q.push(uptr(new T));
  this_thread::sleep_for(std::chrono::seconds(2));
  auto c = q.pop();
  this_thread::sleep_for(std::chrono::seconds(1));
  q.push(uptr(new T));
  auto d = q.pop();
}

int main(int argc, char **argv) {
  //q.push(uptr(new T));
  //auto c = q.pop();
  thread t1(ok), t2(ok), t3(ok), t4(ok);
  t1.join();
  t2.join();
  return 0;
}
