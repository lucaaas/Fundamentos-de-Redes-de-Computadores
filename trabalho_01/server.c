#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "math_functions.h"
#include "socket_configuration.h"
#include "client.h"

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
  char* operator =  malloc (3 * sizeof(char));

  int i = 0;
  for (i = 0; i < 3; i++) {
    operator[i] = message[i];
  }

  return operator;
}

int extractNumber(char* message, int *position) {
  char *numberc = malloc (10 * sizeof(char));

  int i = 0;
  *position += 1;

  printf("message= %s\n", message);
  while (message[*position] != ' ' && *position < strlen(message)) {
    printf("pos = %d\n", *position);
    numberc[i] = message[*position];
    i++;
    *position += 1;
  }

  printf("numberc = %s\n", numberc);
  int number = atoi(numberc);
  return number;
}

int main() {
  int socketServer = createSocket();

  while (TRUE) {
    int status = listenSocket(socketServer, QNTCONNECTION);
    checkListening(status);

    int newSocket = acceptConnection(socketServer);

    char message[TAM_REQ];
    memset(message,'\0', TAM_REQ);
    recv(newSocket, message, TAM_REQ, 0);
    printf("fmessage = %s\n", message);

    char *operator = extractOperator (message);

    int position = 3;
    int number1 = extractNumber(message, &position);
    int number2 = extractNumber(message, &position);

    int result = calculate (operator, number1, number2);

    char *messageResult = malloc (TAM_REQ * sizeof(char));
    if (result != DIV_ERROR) {
      sprintf(messageResult, "%s", "0 ");
      sprintf(messageResult + strlen(messageResult), "%d", result);
    }
    else {
      sprintf(messageResult, "%s", "1 ");
      sprintf(messageResult + strlen(messageResult), "%d", DIV_ERROR);
    }

    send (newSocket, messageResult, TAM_REQ, 0);
    int closed = shutdown(newSocket, 2);

  }

  return 0;
}
