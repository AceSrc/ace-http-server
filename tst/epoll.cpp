#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/epoll.h>
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
  int size = 1024;
  char buf[size + 1];
  int fd = open("log", O_RDWR);
  auto epollfd = epoll_create(1);
  epoll_event ev, events[10];
  ev.events = EPOLLIN;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
  while (true) {
    int nfds = epoll_wait(epollfd, events, 10, -1);
    //cout << nfds << ' ' << events[0].data.fd << ' ' << events[0].events << ' ' << EPOLLIN << ' ' << fd << ' ' << EPOLLHUP << ' ' << fcntl(fd, F_GETFD) << endl;
    for (int i = 0; i < nfds; i++) {
      int rdlen;
      while ((rdlen = read(events[i].data.fd, buf, size)) > 0) {
        cout << buf;
      }
      //cout << "#### " << rdlen << endl;
    }
  }
  return 0;
}
