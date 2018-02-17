/* header file: LOGGER_H */
#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <type.h>
#include <message_queue.h>
#include <fstream>
#include <sstream>

//class Log : public Buffer {
//public:
  //Log();
  //~Log();

  //const char *str();
  //Log &operator <<(const char *s);
//} ;

class Logger {
  //typedef std::unique_ptr<Log> uptr;
  //Buffer buffer;
private:
  std::stringstream ss;
  std::string ip;
  int port;

  message_queue_type *q;

public:
  Logger(message_queue_type *_q = nullptr, const std::string &ip = "Unknown ip", const int port = 0);
  ~Logger();
  
  Logger &operator <<(const std::string &s);
  void format(const char *format, ...);
} ;

class Writer {
private:
  std::ofstream fout;
  std::mutex m;
  std::condition_variable cv;
  bool is_running;
  message_queue_type *q;
  void handle(uptr ptr);

public:
  Writer(message_queue_type *_q);
  ~Writer();

  bool stop();
  void start();
  void set_file(const std::string &filename);
} ;
#endif /* end of include guard: LOGGER_H */

