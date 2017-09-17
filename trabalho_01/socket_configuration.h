#ifndef SOCKET_CONFIGURATION_H
#define SOCKET_CONFIGURATION_H

  struct sockaddr_in configureServerAddr();
  int createSocket();
  int listenSocket(int serverSocket, int qntConnection);
  int acceptConnection(int socket);

#endif
