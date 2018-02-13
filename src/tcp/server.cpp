#include <tcp.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

Server::Server(int port) {
  servfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    printf("Failed to bind\n");
    exit(1);
  }
  if (listen(servfd, MAX_LISTENING) == -1) {
    printf("Failed to listen\n");
    exit(1);
  }
}

Speaker Server::accept_client() {
  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);
  int conn = 10;
  while ((conn = accept(servfd, (struct sockaddr*)&client_addr, &length)) == -1) {
    printf("Failed to connected... Try another work...\n");
  }
  printf("conn: %d\n", conn);
  return Speaker(conn);
}

Server::~Server() {
  close(servfd);
}
