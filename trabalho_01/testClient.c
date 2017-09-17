#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket_configuration.h"

int main(int argc, char const *argv[]) {
  int clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serverAddr = configureServerAddr();

  socklen_t addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  char message[30];
  scanf("%s", message);

  send(clientSocket, message, 11, 0);
  return 0;
}
