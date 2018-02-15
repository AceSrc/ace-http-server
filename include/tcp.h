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

class Server {
  enum {
    MAX_LISTENING = 20
  } ;
private:
  int servfd;

public:
  Server(int proxy = 8080);
  ~Server();
  User accept_client();
} ;

class Client {
private:
  Speaker speaker;

public:
  Client(const char *ip, int proxy = 8080);
  char *recv();
} ;
#endif


