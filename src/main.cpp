#include <tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <response.h>
#include <iostream>
#include <request.h>
using namespace std;

int main() {
  //printf("Running\n");
  //TCPServer server(8080);
  //for (int i = 0; i < 10; i++) {
    //auto s = server.accept_client();
    //printf("Accept\n");
    //char buf[1024];
    //s.get_content(buf, 1024);

    //Response res;
    //res.update_content("HelloWorld");
    //cout << buf << endl;
    //s << res.str();
  //}
  //}
  //Request r;
  //r.destr("subject");
  //r.destr("Get /index.html?ace=1&&src=2 HTTP/1.1\r\na: 1\r\nb: 2\r\n\r\nHelloWorld");
  //r.print();
  TCPClient client("127.0.0.1", 8080);
  cout << client.get_content() << endl;
  return 0;
}

