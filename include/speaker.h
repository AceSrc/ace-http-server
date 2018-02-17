/* header file: SPEAKER_H */
#ifndef __SPEAKER_H_
#define __SPEAKER_H_

#include <unistd.h>
#include <string>

class Speaker {
  enum {
    DEFAULT_BUF_SIZE = 1,
    CHAR_SET_SIZE = 256
  } ;

private:
  int fd;
  int w_fd;

  int buf_size;
  char *buf;
  int read_len;
  int loaded_len;

  bool is_vaild[CHAR_SET_SIZE];

public:
  Speaker(int fd = 1);
  Speaker(Speaker &&);
  ~Speaker();

  void setfd(int _fd);
  void setwfd(int _fd);

  template<typename T>
    void send_struct(const T &s) {
      if (!w_fd) write(fd, &s, sizeof(T));
      else write(w_fd, &s, sizeof(T));
    }

  void send(const char *s);
  void send(const char *s, int size);
  void send(const std::string &s);
  void send(const std::string &s, int size);

  void recv_once();
  std::string split(const char *s, int offset = 0);
  std::string recv(int size);
  template<typename T>
    T recv_struct() {
      int size = sizeof(T);
      while (loaded_len + size > read_len) {
        recv_once();
      }
      T rt = *((T *)(buf + loaded_len));
      loaded_len += size;
      return rt;
    }

  char test(int inx = 0);
  void skip(int offset = 0);

  Speaker &operator=(const Speaker &) = delete;
  Speaker &operator=(Speaker &&);
} ;


#endif /* end of include guard: SPEAKER_H */

