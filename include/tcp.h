#ifndef __TCP_H__
#define __TCP_H__

#include <speaker.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <user.h>
#include <mutex>

class Server {
  enum {
    MAX_LISTENING = 20
  } ;
private:
  int servfd;
  std::mutex m;

public:
  Server();
  ~Server();
  User accept_client();
  void start(int port = 8080);
} ;

class Client {
private:
  Speaker speaker;

public:
  Client(const char *ip, int proxy = 8080);
  char *recv();
} ;
#endif


