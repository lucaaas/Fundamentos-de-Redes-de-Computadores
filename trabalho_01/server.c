#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "math_functions.h"
#include "socket_configuration.h"

#define QNTCONNECTION 5
#define TRUE 1

void checkListening (int status) {
  if(status == 0) {
    printf("Listening\n");
  }
  else {
    printf("Error on listening\n");
  }
}

char* extractOperator(char *message) {
  char[3] operator;

  for (int i = 0; i < 3; i++) {
    operator[i] = message[i];
  }

  return operator;
}

int extractNumber1(char* message) {
  
}

int main() {
  int socketServer = createSocket();

  while (TRUE) {
    int status = listenSocket(socketServer, QNTCONNECTION);
    checkListening(status);

    int newSocket = acceptConnection(socketServer);

    char message[30];
    recv(newSocket, message, 11, 0);


    int closed = shutdown(newSocket, 2);

  }

  return 0;
}
