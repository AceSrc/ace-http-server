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
using namespace std;

Queue<Buffer> message_queue;
Writer writer(&message_queue);
Logger logger(&message_queue);

void start_writer() {
  writer.start();
}

void start_server() {
  Server server; 
  while (true) {
    auto speaker = server.accept_client();
    //Request req;
    //cout << speaker.recv() << endl;
    //req.destr(speaker.recv());
    //req.print();
    //speaker.send("HelloWorld");
    //delete &speaker;
  }
}

struct T {
  char s[4];
};

void test_fastcgi() {
  Fastcgi fastcgi("127.0.0.1", 9000);
  params_type mp;

  //mp["a"] = "";
  //mp["SCRIPT_FILENAME"]="/usr/share/nginx/html/a.php";
  //mp["SCRIPT_FILENAME"] = "/root/http/html/a.php";
  mp["SCRIPT_FILENAME"] = "/html/a.php";
  mp["QUERY_STRING"]="";
  mp["REQUEST_METHOD"]="GET";
  mp["CONTENT_TYPE"]="";
  mp["CONTENT_LENGTH"]="";
  mp["SCRIPT_NAME"]="/a.php";
  mp["REQUEST_URI"]="/a.php";
  mp["DOCUMENT_URI"]="/a.php";
  //mp["DOCUMENT_ROOT"]="/usr/share/nginx/html";
  mp["DOCUMENT_ROOT"]="/root/http/html/";
  mp["SERVER_PROTOCOL"]="HTTP/1.1";
  mp["REQUEST_SCHEME"]="http";
  mp["GATEWAY_INTERFACE"]="CGI/1.1";
  mp["SERVER_SOFTWARE"]="nginx/1.12.1";
  mp["REMOTE_ADDR"]="127.0.0.1";
  mp["REMOTE_PORT"]="36102";
  mp["SERVER_ADDR"]="127.0.0.1";
  mp["SERVER_PORT"]="80";
  mp["SERVER_NAME"]="localhost";
  mp["REDIRECT_STATUS"]="200";
  mp["HTTP_HOST"]="localhost";
  mp["HTTP_CONNECTION"]="keep-alive";
  mp["HTTP_CACHE_CONTROL=max-age"]="0";
  mp["HTTP_UPGRADE_INSECURE_REQUESTS"]="1";
  mp["HTTP_USER_AGENT"]="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36";
  mp["HTTP_ACCEPT=text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q"]="0.8";
  //mp["HTTP_ACCEPT_ENCODING"]="gzip, deflate, br";
  mp["HTTP_ACCEPT_ENCODING"]="";
  mp["HTTP_ACCEPT_LANGUAGE=en-US,en;q"]="0.9";
  fastcgi.send(0x0001, mp, "HelloWorld");
  fastcgi.recv();
  cout << "stdout" << endl;
  cout << fastcgi.get_stdout() << endl;
  cout << "stderr" << endl;
  cout << fastcgi.get_stderr() << endl;
}

int main() {
  //Buffer buffer;
  //string st;
  //for (int i = 0; i < 100000000; i++) {
    //st += "Hello";
    //buffer.append("Hello");
  //}
  //Buffer buffer;
  //buffer << "HelloWorld\r\n" << 123;
  //cout << buffer.str() << endl;
  //test_fastcgi();
  //Request req;
  //User user;
  //Speaker speaker;

  read_config();
  Server server(8080);
  while (true) {auto p = server.accept_client();}
  
  //auto fd = open("log", O_RDONLY);
  //User user(fd, &message_queue);
  //cout << "H" << endl;
  //user.print();
  //Request re;
  //re.destr(&speaker);

  //writer.set_file(GLOBALS["log_file"]);
  //thread writer_thread(start_writer);
  //thread server_thread(start_server);

  //while (true) {
    //char ch;
    //cin >> ch;
    //if (ch == 'q') break;
  //}
  
  //writer.stop();
  //writer_thread.join();
  //server_thread.join(); 
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

