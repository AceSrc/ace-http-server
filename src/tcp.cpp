#include <tcp.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int TCPBuf::overflow(int _ch) {
  write(fd, &_ch, 1);
  return _ch;
}

std::streamsize TCPBuf::xsputn(const char *s, std::streamsize n) {
  int size = strlen(s);
  if (size < n) n = size;
  for (int i = 0; i < n; i++) overflow(s[i]);
  return std::streamsize(n);
}

void TCPBuf::set_fd(int _fd) {
  fd = _fd; 
}

TCPBuf::TCPBuf(TCPBuf &&tmp) {}
TCPBuf::TCPBuf() {}

TCPStream::TCPStream() : tcpbuf() {
  this->init(&tcpbuf);
}

TCPStream::TCPStream(TCPStream &&tmp) {}

TCPStream::TCPStream(int _fd) : fd(_fd) {
  tcpbuf.set_fd(fd);
  this->init(&tcpbuf);
}

static int fd_read(int fd, void *buf, int buf_size) {
  return read(fd, buf, buf_size);
}

template<typename T>
TCPStream &TCPStream::operator>>(T &x) {
  fd_read(this->fd, &x, sizeof(T));
  return *this;
}

int TCPStream::get_content(void *buf, const int buf_size) {
  fd_read(this->fd, buf, buf_size);
  return 0;
}

TCPServer::TCPServer(int port) {
  servfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    printf("Failed to bind\n");
    exit(1);
  }
  if (listen(servfd, MAX_LISTENING) == -1) {
    printf("Failed to listen\n");
    exit(1);
  }
}

TCPStream TCPServer::accept_client() {
  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);
  int conn = 10;
  while ((conn = accept(servfd, (struct sockaddr*)&client_addr, &length)) == -1) {
    printf("Failed to connected... Try another work...\n");
  }
  printf("conn: %d\n", conn);
  return TCPStream(conn);
}

TCPBuf::~TCPBuf() {
  close(fd);
}

TCPServer::~TCPServer() {
  close(servfd);
}

const int DEFAULT_BUF_SIZE = 1024;
TCPClient::TCPClient(const char *ip, int proxy) : buf(NULL), buf_size(DEFAULT_BUF_SIZE) {
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(proxy);

  clientfd = socket(PF_INET, SOCK_STREAM, 0);
  connect(clientfd, (sockaddr *)&addr, sizeof(sockaddr));
}

TCPClient::~TCPClient() {
  close(clientfd);
}

#include <iostream>
using namespace std;
char *TCPClient::get_content() {
  if (!buf) buf = (char *)malloc(buf_size + 1);

  p = buf;
  int read_len = read(clientfd, p, buf_size);
  p = p + read_len;
  //assert(read_len <= buf_size);
  while (read_len != 0) {
    if (read_len >= buf_size) {
      buf = (char *)realloc(buf, buf_size * 2 + 1);
      buf_size *= 2;
    }
    int rt = read(clientfd, p, buf_size - read_len);
    read_len += rt;
    p += rt;
    if (rt == 0) break;
  }
  *p = '\0';
  return buf;
}
