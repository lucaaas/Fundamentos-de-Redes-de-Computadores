#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "socket_configuration.h"

struct sockaddr_in configureServerAddr() {
  struct sockaddr_in serverAddr;

  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(2306);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  return serverAddr;
}

int createSocket() {
  int socketServer, newSocket;
  struct sockaddr_storage serverStorage;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  socketServer = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr = configureServerAddr();

  /*---- Bind the address struct to the socket ----*/
  bind(socketServer, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  int status = listen(socketServer, 5);
  if(status == 0) {
    printf("Listening\n");
  }
  else {
    printf("Error\n");
  }

  /*---- Accept call creates a new socket for the incoming connection ----*/
  socklen_t addr_size;
  addr_size = sizeof serverStorage;
  newSocket = accept(socketServer, (struct sockaddr *) &serverStorage, &addr_size);

  return socketServer;
}
