#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MSGSIZE 32
#define MAX_MSGSIZE 1024
#define BUF MAX_MSGSIZE + 1

int main(int argc, char *argv[]) {
  int serversock;
  int clientsock;
  struct sockaddr_in serverSockAddr;
  struct sockaddr_in clientSockAddr;
  unsigned int port;
  unsigned int clientlen;
  char recvbuf[BUF];
  int index, sendsize;

  if (argc != 2) {
    fprintf(stderr, "usage : ./a.out [port number]\n");
    exit(EXIT_FAILURE);
  }
  if ((port = (unsigned int)atoi(argv[1])) == 0) {
    fprintf(stderr, "server port must be larger than 0\n");
    exit(EXIT_FAILURE);
  }
  if ((serversock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket() failed.");
    exit(EXIT_FAILURE);
  }
  memset(&serverSockAddr, 0, sizeof(serverSockAddr));
  serverSockAddr.sin_family = PF_INET;
  serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverSockAddr.sin_port = htons(port);

  if (bind(serversock, (struct sockaddr *)&serverSockAddr,
           sizeof(serverSockAddr)) < 0) {
    perror("bind() failed.");
    exit(EXIT_FAILURE);
  }
  if (listen(serversock, 3) < 0) {
    perror("bind() failed.");
    exit(EXIT_FAILURE);
  }
  while (1) {
    clientlen = sizeof(clientSockAddr);
    if ((clientsock = accept(serversock, (struct sockaddr *)&clientSockAddr,
                             &clientlen)) < 0) {
      perror("accept() failed.");
      exit(EXIT_FAILURE);
    };
    printf("connected form %s.\n", inet_ntoa(clientSockAddr.sin_addr));

    while (1) {
      if ((index = recv(clientsock, recvbuf, BUF, 0)) < 0) {
        perror("recv() failed.");
        exit(EXIT_FAILURE);
      } else if (index == 0) {
        fprintf(stderr, "connection lost.\n");
        break;
      }

      if ((sendsize = send(clientsock, recvbuf, index, 0)) < 0) {
        perror("sed() failed.");
        exit(EXIT_FAILURE);
      } else if (sendsize == 0) {
        fprintf(stderr, "connection lost.\n");
        break;
      }
    }
    close(clientsock);
  }
  close(serversock);

  return EXIT_SUCCESS;
}
