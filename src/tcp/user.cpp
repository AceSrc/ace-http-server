#include <user.h>
#include <iostream>
using namespace std;

User::User() {

}

User::User(const int fd, message_queue_type *q, const std::string &ip, const int port) : speaker(fd), request(&speaker), logger(q, ip, port), response(&request, &speaker, &logger) {
}

void User::start() {
  logger << "Connected";
  request.destr();
  logger << "http request parsed";
  response.send_resource();
  logger << "Resource sent";
}

void User::print() {
  request.print();
}

User::~User() {
}


