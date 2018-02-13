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
using namespace std;

Queue<Buffer> message_queue;
Writer writer(&message_queue);
Logger logger(&message_queue);
void start() {
  writer.start();
}
int main() {
  logger << "HelloWorld";
  writer.set_file("log");
  thread th(start);
  char ch;
  cin >> ch;
  writer.stop();
  th.join();
  //Client client("127.0.0.1", 8080);
  //cout << client.recv() << endl;
  //return 0;
  //Fastcgi cgi("127.0.0.1", 9000);
  //params_type params;
  //params["a"] = (const char *)"1";
  //params["b"] = (const char *)"2";
  //params["SCRIPT_FILENAME"] = "index.html";
  //const char *s = "HelloWorld";
  //cgi.send(0xffff, params, s);
  //cgi.recv();
  //cout << cgi.get_stdout() << endl;
  return 0;
}

