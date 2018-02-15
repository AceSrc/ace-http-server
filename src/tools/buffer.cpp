#include <tools.h>
#include <buffer.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define INITIAL_BUF_LEN 128
Buffer::Buffer() : cpos(0), buf_size(INITIAL_BUF_LEN) {
  buf = (char *)malloc(buf_size + 1);
  p = buf;
}

Buffer::~Buffer() {
  if (buf) free(buf);
}

void Buffer::clear() {
  p = buf;
  cpos = 0;
}

void Buffer::append(const char *s, int wrlen) {
  int size;
  if (wrlen == -1) size = strlen(s); 
  else {
    size = wrlen;
  }

  while (size + cpos >= buf_size) {
    buf_size *= 2;
    buf = (char *)realloc(buf, buf_size + 1);
    p = buf + cpos;
  } 

  memcpy(p, s, size);
  p += size;
  cpos += size;
  *p = '\0';
}

char *Buffer::str(int offset) const {
  return buf + offset;
}

int Buffer::size() const {
  return cpos;
}

Buffer &Buffer::operator<<(const char *s) {
  this->append(s);
  return *this;
}

Buffer &Buffer::operator<<(const int &x) {
  const int MAX_INT_LEN = 32;
  char *buf = (char *)malloc(MAX_INT_LEN);
  sprintf(buf, "%d", x);
  this->append(buf);
  free(buf);
  return *this;
}

