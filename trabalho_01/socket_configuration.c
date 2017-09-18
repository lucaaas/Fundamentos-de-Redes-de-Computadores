#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "socket_configuration.h"

#define IPADRESS "127.0.0.1"
#define PORTNUMBER 12000


struct sockaddr_in configureServerAddr() {
  struct sockaddr_in serverAddr;

  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(PORTNUMBER);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr(IPADRESS);
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  return serverAddr;
}

int createSocket() {
  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  int newSocket = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr = configureServerAddr();

  /*---- Bind the address struct to the socket ----*/
  bind(newSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  return newSocket;
}

int acceptConnection(int socketServer) {
  struct sockaddr_storage serverStorage;
  socklen_t addr_size = sizeof serverStorage;
  int newSocket = accept(socketServer, (struct sockaddr *) &serverStorage, &addr_size);

  return newSocket;
}

// Listen on the socket, with 5 max connection requests queued
int listenSocket(int serverSocket, int qntConnection) {
  int status = listen(serverSocket, qntConnection);

  return status;
}
