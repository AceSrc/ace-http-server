#ifndef __TCP_H__
#define __TCP_H__

#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Speaker {
  enum {
    DEFAULT_BUF_SIZE = 128
  } ;

private:
  int fd;

  int buf_size;
  char *buf;

public:
  Speaker(int fd = 1);
  Speaker(Speaker &&);
  ~Speaker();

  void setfd(int _fd);

  template<typename T>
    void send(const T &s) {
      write(fd, &s, sizeof(T));
    }

  void send(const char *s);
  void send(const char *s, int size);

  template<typename T>
    T recv() {
      T rt;
      void *p = (void *)&rt;
      int size = sizeof(T);
      int cur = 0;
      while (cur < size) {
        int current_read_len = read(fd, p, size - cur);
        p += current_read_len;
        cur += current_read_len;
      }
      return rt;
    }

  char *recv();
  char *recv(int size);

  Speaker &operator=(const Speaker &) = delete;
  Speaker &operator=(Speaker &&);
} ;

class Server {
  enum {
    MAX_LISTENING = 20
  } ;
private:
  int servfd;

public:
  Server(int proxy = 8080);
  ~Server();
  Speaker accept_client();
} ;

class Client {
private:
  Speaker speaker;

public:
  Client(const char *ip, int proxy = 8080);
  char *recv();
} ;
#endif

