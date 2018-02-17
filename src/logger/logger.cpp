#include <logger.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>

#include <iostream>

Logger::Logger(message_queue_type *_q, const std::string &ip, const int port) : q(_q), ip(ip), port(port) {}

Logger::~Logger() {
}

Logger &Logger::operator <<(const std::string &s) {
  const int BUFLEN = 30;
  char buffer[BUFLEN];
  time_t t = time(0);
  strftime(buffer, BUFLEN, " %F %T ", localtime(&t));

  //std::cout << ip << ' ' << port << ' ' << buffer << ' ' << s << std::endl;
 
  //std::cout << "Before " << s << std::endl;
  ss.str("");
  ss << ip << ' ' << port << ' ' << buffer << ' ' << s << std::endl;
  //std::cout << "After " << s << std::endl;
  //std::cout << ss.str() << std::endl;
  q->push(uptr(new std::string(ss.str())));
  return *this;
}

void Logger::format(const char *format, ...) {
  /* TODO: buffer safety */
  char buf[1024];
  va_list ap;
  va_start(ap, format);
  vsprintf(buf, format, ap);
  *this << std::string(buf);
  va_end(ap);
}

Writer::Writer(message_queue_type *_q) : is_running(false), q(_q) {
}

Writer::~Writer() {
}

void Writer::start() {
  is_running = true;

  while (true) {
    auto ptr = q->pop();
    if (ptr) handle(std::move(ptr));
    else if (!is_running) break;
  }
}

bool Writer::stop() {
  is_running = false;
  return true;
}

void Writer::handle(uptr ptr) {
  if (fout.is_open()) {
    fout << *ptr.get();
    std::cout << *ptr.get();
    fout.flush();
  }
}

void Writer::set_file(const std::string &filename) {
  if (fout.is_open()) fout.close();
  fout.open(filename, std::ios_base::app);
}

