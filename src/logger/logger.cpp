#include <logger.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>

#include <iostream>
using namespace std;
Logger::Logger(Queue<Buffer> *_q) : ip(NULL), q(_q) {}
Logger::~Logger() {
  if (ip) free((void *)ip);
}

#include <iostream>
using namespace std;
Logger &Logger::operator <<(const char *s) {
  const int BUFLEN = 30;
  char buffer[BUFLEN];
  time_t t = time(0);
  strftime(buffer, BUFLEN, " %F %T ", localtime(&t));
 
  Buffer *buf = new Buffer();
  //cout << buf << ' ' << ip << endl;
  if (ip) buf->append(ip);
  buf->append(buffer);
  buf->append(s);
  buf->append("\n");
  q->push(std::unique_ptr<Buffer>(buf));
  return *this;
}

void Logger::format(const char *format, ...) {
  char buf[1024];
  va_list ap;
  va_start(ap, format);
  vsprintf(buf, format, ap);
  *this << buf;
  va_end(ap);
}

//void Logger::open(const char *s) {
  //if (fd) close(fd);
  //fd = open(s, O_APPEND);
  //if (fcntl(fd, F_GETFD) == -1) {
    //printf("Can't open log file\n");
    //return ;
  //}
//}

//void Logger::handle(const uptr &ptr) {
  //write(fd, ptr.get()->str(), ptr.get()->size());
  //ptr.release();
//}

Writer::Writer(Queue<Buffer> *_q) : fd(0), is_running(false), q(_q) {
}

Writer::~Writer() {
  if (fd) {close(fd); cout << "Close" << endl;}
}

void Writer::start() {
  is_running = true;
  //std::unique_lock<std::mutex> lock(m);
  //lock.unlock();

  while (true) {
    //lock.lock();
    //cv.wait(lock);
    auto ptr = q->pop();
    if (!is_running) break;
    if (ptr) handle(std::move(ptr));
  }
}

bool Writer::stop() {
  is_running = false;
  q->once();
  //cv.notify_one();
  return true;
}

void Writer::handle(std::unique_ptr<Buffer> ptr) {
  //cout << fd << ' ' << ptr.get()->str() << ' ' << ptr.get()->size() << endl;
  if (fd) {
    write(fd, ptr.get()->str(), ptr.get()->size());
    cout << ptr.get()->str();
    //cout << "Writing.." << endl;
  }
}

//extern "C" int open(const char *, int);
void Writer::set_file(const char *s) {
  if (fd) close(fd);
  fd = open(s, O_RDWR | O_APPEND);
}

