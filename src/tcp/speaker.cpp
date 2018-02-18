#include <unistd.h>
#include <tcp.h>
#include <cstring>
#include <iostream>
using namespace std;

Speaker::Speaker(int fd) : fd(fd), w_fd(0), buf_size(DEFAULT_BUF_SIZE), buf(nullptr), read_len(0), loaded_len(0) {
  memset(is_vaild, false, sizeof(is_vaild));
}

void Speaker::setfd(int _fd) {
  fd = _fd;
}

void Speaker::setwfd(int _fd) {
  w_fd = _fd;
}

void Speaker::send(const char *s) {
  if (!w_fd) write(fd, s, strlen(s));
  else write(w_fd, s, strlen(s));
}


void Speaker::send(const char *s, int size) {
  if (!w_fd) write(fd, s, size);
  else write(w_fd, s, size);
}

void Speaker::send(const std::string &s) {
  send(s.c_str(), s.size());
}

void Speaker::send(const string &s, int size) {
  send(s.c_str(), size);
}

void Speaker::recv_once() {
  if (!buf) buf = (char *)malloc(buf_size); 
  if (read_len == buf_size) {
    buf_size *= 2;
    buf = (char *)realloc(buf, buf_size); 
  }
  int rd = 0;
  while ((rd = read(fd, buf + read_len, buf_size - read_len)) == 0) {
  }
  read_len += rd;
}

std::string Speaker::split(const char *s, int offset) {
  for (int i = strlen(s) - 1; i >= 0; i--) 
    if ((int)s[i] >= 0 && (int)s[i] <= CHAR_SET_SIZE) is_vaild[ (int)s[i] ] = true;

  int test_len = 0;
  while (true) {
    if (test_len + loaded_len == read_len) recv_once();
    if (is_vaild[ (int)*(buf + test_len + loaded_len) ]) break;
    test_len++;
  }
  char *rt = buf + loaded_len + offset;
  *(buf + test_len + loaded_len) = '\0';
  //std::cout << rt << ' ' << test_len << std::endl;
  loaded_len += test_len + 1;

  for (int i = strlen(s) - 1; i >= 0; i--) 
    if ((int)s[i] >= 0 && (int)s[i] <= CHAR_SET_SIZE) is_vaild[ (int)s[i] ] = false;
  return std::string(rt, test_len - offset);
}

char Speaker::test(int inx) {
  while (loaded_len + inx > read_len) recv_once();
  return *(buf + loaded_len + inx);
}

void Speaker::skip(int offset) {
  while (loaded_len + offset > read_len) recv_once();
  loaded_len += offset;
}

std::string Speaker::recv(int size) {
  while (loaded_len + size > read_len) recv_once();
  char *rt = buf + loaded_len;
  loaded_len += size;
  return std::string(rt, size);
}

Speaker::~Speaker() {
  if (fd) close(fd);
  if (w_fd) close(w_fd);
  if (buf) free(buf);
}

Speaker &Speaker::operator=(Speaker &&x) {
  this->fd = x.fd;
  this->w_fd = x.w_fd;
  this->buf_size = x.buf_size;
  this->buf = x.buf;
  this->read_len = x.read_len;
  this->loaded_len = x.loaded_len;
  x.buf = nullptr;
  x.fd = 0;
  x.w_fd = 0;
  return *this;
}

Speaker::Speaker(Speaker &&x) {
  this->fd = x.fd;
  this->w_fd = x.w_fd;
  this->buf_size = x.buf_size;
  this->buf = x.buf;
  this->read_len = x.read_len;
  this->loaded_len = x.loaded_len;
  x.buf = nullptr;
  x.fd = 0;
  x.w_fd = 0;
}

