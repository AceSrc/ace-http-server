/* header file: USER_H */
#ifndef __USER_H_
#define __USER_H_

#include <response.h>
#include <request.h>
#include <speaker.h>
#include <logger.h>
#include <type.h>

class User {
private:
  Speaker speaker;
  Request request;
  Response response;
  Logger logger;

public:
  User();
  User(const int fd, message_queue_type *_q, const std::string &ip = "unknown ip", const int port = 0);
  User(User &&x);
  User(const User &x) = delete;
  ~User();

  void start();

  void print();
  User &operator=(User &&x);
  User &operator=(const User &x) = delete;
} ;

#endif /* end of include guard: USER_H */
