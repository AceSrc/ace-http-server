/* header file: LOGGER_H */
#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <buffer.h>
#include <message_queue.h>

//class Log : public Buffer {
//public:
  //Log();
  //~Log();

  //const char *str();
  //Log &operator <<(const char *s);
//} ;

class Logger {
  //typedef std::unique_ptr<Log> uptr;
  Buffer buffer;
private:
  const char *ip;
  int port;

  Queue<Buffer> *q;

public:
  Logger(Queue<Buffer> *_q = nullptr);
  ~Logger();
  
  //void open(const char *s);
  Logger &operator <<(const char *s);
  void format(const char *format, ...);
} ;

class Writer {
private:
  int fd;
  //std::mutex m;
  //std::condition_variable cv;
  bool is_running;
  Queue<Buffer> *q;
  void handle(std::unique_ptr<Buffer> ptr);

public:
  Writer(Queue<Buffer> *_q);
  ~Writer();

  bool stop();
  void start();
  void set_file(const char *s);
} ;
#endif /* end of include guard: LOGGER_H */

