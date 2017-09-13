#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "math_functions.h"
#include "socket_configuration.h"

int main() {
  int socket = createSocket();
  char message[30];
  recv(socket, message, 11, 0);
  return 0;
}
