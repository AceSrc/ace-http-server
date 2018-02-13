#include <tcp.h>

Speaker::Speaker(int fd) : fd(fd), buf_size(DEFAULT_BUF_SIZE), buf(nullptr) {
  //printf("%d\n", fcntl(fd, F_GETFD));
}

void Speaker::setfd(int _fd) {
  fd = _fd;
}

//template <typename T>
//void Speaker::send(const T &s) {
  //write(fd, &s, sizeof(T));
//}

void Speaker::send(const char *s) {
  write(fd, s, strlen(s));
}

void Speaker::send(const char *s, int size) {
  write(fd, s, size);
}

//template<typename T>
//T Speaker::recv() {
  //T rt;
  //void *p = (void *)&rt;
  //int size = sizeof(T);
  //int cur = 0;
  //while (cur < size) {
    //int current_read_len = read(fd, p, size - cur);
    //p += current_read_len;
    //cur += current_read_len;
  //}
  //return rt;
//}

char *Speaker::recv() {
  if (buf == nullptr) buf = (char *)malloc(buf_size + 1);
  char *p = buf;
  int read_len = 0;
  if (fcntl(fd, F_GETFD) == -1) {
    printf("Socket invaild\n");
    exit(1);
  }

  while (true) {
    int current_read_len = read(fd, p, buf_size - read_len);
    if (current_read_len < 0) {
      printf("Failed to read\n");
      exit(1);
    }
    if (current_read_len == 0) break;
    read_len += current_read_len;
    p += current_read_len;
    if (read_len == buf_size) {
      buf_size *= 2;
      buf = (char *)realloc(buf, buf_size + 1);
      p = buf + read_len;
    }
  }
  *p = '\0';
  return buf;
}

char *Speaker::recv(int size) {
  if (buf == nullptr) buf = (char *)malloc(buf_size + 1);
  if (buf_size < size) {
    buf_size = size;
    buf = (char *)realloc(buf, buf_size);
  }

  char *p = buf;
  int read_len = 0;
  if (fcntl(fd, F_GETFD) == -1) {
    printf("Socket invaild\n");
    exit(1);
  }

  while (read_len < size) {
    int current_read_len = read(fd, p, size - read_len);
    read_len += current_read_len;
    p += current_read_len;
  }
  *p = '\0';
  return buf;
}

Speaker::~Speaker() {
  close(fd);
  free(buf);
}

Speaker &Speaker::operator=(Speaker &&x) {
  this->fd = x.fd;
  this->buf_size = x.buf_size;
  this->buf = x.buf;
  x.buf = nullptr;
  return *this;
}

Speaker::Speaker(Speaker &&x) {
  this->fd = x.fd;
  this->buf_size = x.buf_size;
  this->buf = x.buf;
  x.buf = nullptr;
}
