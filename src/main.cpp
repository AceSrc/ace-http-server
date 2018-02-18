#include <user.h>
#include <global.h>
#include <thread>
#include <tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <response.h>
#include <iostream>
#include <request.h>
#include <type.h>
#include <fastcgi.h>
#include <logger.h>
#include <message_queue.h>
#include <config.h>
#include <signal.h>
using namespace std;

message_queue_type message_queue;
Writer writer(&message_queue);
Logger logger(&message_queue);

thread writer_thread;

Server server;

void start_writer() {
  writer.start();
}

void start_user(int id) {
  while (true) {
    auto p = server.accept_client();
    std::cout << "Accept: " << id << std::endl;
    p.start();
  }
}

void on_int_exit(int) {
  cout << "Shutting down server..." << endl;
  message_queue.stop();
  writer.stop();
  if (writer_thread.joinable()) writer_thread.join();
  cout << "logger system stoped" << endl;
  exit(0);
}

int main() {
  signal(SIGINT, on_int_exit);

  read_config();
  writer.set_file(GLOBALS["log_file"]);

  writer_thread = thread(start_writer);

  server.start(stoi(GLOBALS["port"]));

  vector<thread> pools(stoi(GLOBALS["max_user"]));
  int id = 0;
  for (auto &x : pools) {
    x = thread(start_user, id);
    id++;
  }
  while (true) {
    char ch;
    scanf("%c", &ch);
    if (ch == 'l') {
      read_config();
    }
  }
  return 0;
}

