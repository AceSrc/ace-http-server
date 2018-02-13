#include <buffer.h>

#include <stdlib.h>
#include <cstring>

#define MAX_BUF_LEN 128
Buffer::Buffer() : cpos(0), buf_size(MAX_BUF_LEN) {
  buf = (char *)malloc(MAX_BUF_LEN + 1);
  p = buf;
}

Buffer::~Buffer() {
  free(buf);
}

void Buffer::clear() {
  p = buf;
  cpos = 0;
}

#include <cstdio>
void Buffer::append(const char *s, int wrlen) {
  int size;
  if (wrlen == -1) size = strlen(s); 
  else {
    size = wrlen;
    //printf("%d\n", size);
  }
  while (size + cpos >= buf_size) {
    buf_size *= 2;
    buf = (char *)realloc(buf, buf_size);
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
