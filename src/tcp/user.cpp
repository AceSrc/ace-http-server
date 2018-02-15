#include <user.h>
#include <iostream>
using namespace std;

User::User() {

}

User::User(const int fd, message_queue_type *q) : speaker(fd), request(&speaker), logger(q), response(&request, &speaker) {
  request.destr();
  response.send_resource();
}

void User::print() {
  request.print();
}

User::~User() {
}


