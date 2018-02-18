#include <user.h>
#include <global.h>
#include <tcp.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <request.h>
#include <response.h>
#include <string>

Server::Server() {

}

void Server::start(int port) {
  servfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  int iSockOptVal = 1;
  if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &iSockOptVal, sizeof(iSockOptVal)) == -1) {
    printf("Failed to setsockopt\n");
    exit(1);
  }

  while (bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    sleep(1);
    printf("Failed to bind(%d)\n", errno);
  }
  printf("Start waiting...\n");
  if (listen(servfd, MAX_LISTENING) == -1) {
    printf("Failed to listen\n");
    exit(1);
  }
}

User Server::accept_client() {
  std::lock_guard<std::mutex> lock(m);

  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);
  int conn = 0;
  while ((conn = accept(servfd, (struct sockaddr*)&client_addr, &length)) == -1) {
    printf("Failed to connected... Try another work...\n");
  }
  //logger.format("connect fd: %d", conn);
  return User(conn, &message_queue, std::string(inet_ntoa(client_addr.sin_addr)), ntohs(client_addr.sin_port));
}

//#include <iostream>
//using namespace std;
Server::~Server() {
  close(servfd);
}
