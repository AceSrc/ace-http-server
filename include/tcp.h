#ifndef __TCP_H__
#define __TCP_H__

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

const int MAX_LISTENING = 20;
class TCPBuf : public std::streambuf {
private:
  int fd;
protected:
  int overflow(int ch) override;
  std::streamsize xsputn(const char *s, std::streamsize n) override;

  TCPBuf(const TCPBuf &) = delete;
  TCPBuf &operator=(const TCPBuf &) = delete;
  TCPBuf(TCPBuf &&);
  TCPBuf &operator=(TCPBuf &&) = delete;
  operator bool();
public:
  TCPBuf();
  ~TCPBuf();
  void set_fd(int _fd);
} ;

class TCPStream : public std::iostream {
  TCPBuf tcpbuf;
  int fd;
public:
  TCPStream();
  TCPStream(int fd);

  TCPStream(const TCPStream &) = delete;
  TCPStream &operator=(const TCPStream &) = delete;
  TCPStream(TCPStream &&);
  TCPStream &operator=(TCPStream &&) = delete;
  operator bool();

  template<typename T>
  TCPStream &operator>>(T &x);

  int get_content(void *buf, const int buf_size);
} ;

class TCPServer {
private:
  int servfd;
public:
  TCPServer(int proxy = 8080);
  ~TCPServer();
  TCPStream accept_client();
} ;

class TCPClient {
private:
  int clientfd;
  char *buf;
  int buf_size;
  char *p;
public:
  TCPClient(const char *ip, int proxy = 8080);
  ~TCPClient();
  char *get_content();
} ;
#endif

