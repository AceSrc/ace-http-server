#include <tcp.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

Client::Client(const char *ip, int proxy) {
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(proxy);

  int clientfd = socket(PF_INET, SOCK_STREAM, 0);
  connect(clientfd, (sockaddr *)&addr, sizeof(sockaddr));
  speaker.setfd(clientfd);
}

char *Client::recv() {
  return speaker.recv();
}
