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
  int sock;
  struct sockaddr_in serverSockAddr;
  unsigned short port;
  char buf[BUF];
  char sendbuf[BUF];
  int totalrecv, bufrecv;

  if (argc != 3) {
    fprintf(stderr, "usage : ./a.out [serverIP] [serverPort]");
    exit(EXIT_FAILURE);
  }
  memset(&serverSockAddr, 0, sizeof(serverSockAddr));
  serverSockAddr.sin_family = PF_INET;
  if (inet_aton(argv[1], &serverSockAddr.sin_addr) == 0) {
    fprintf(stderr, "invaled network address.");
    exit(EXIT_FAILURE);
  }
  if ((port = (unsigned short)atoi(argv[2])) < 0) {
    fprintf(stderr, "invaled port number.");
    exit(EXIT_FAILURE);
  }
  serverSockAddr.sin_port = htons(port);

  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("sock() failed. local");
    exit(EXIT_FAILURE);
  }
  if (connect(sock, (struct sockaddr *)&serverSockAddr,
              sizeof(serverSockAddr)) < 0) {
    perror("connect() failed.");
    exit(EXIT_FAILURE);
  }
  printf("connected to %s.\n", inet_ntoa(serverSockAddr.sin_addr));

  while (1) {
    printf("enter charactors : ");
    if (fgets(sendbuf, BUF, stdin) == NULL) {
      fprintf(stderr, "invailed input string.");
      exit(EXIT_FAILURE);
    }
    if (send(sock, sendbuf, strlen(sendbuf), 0) < 0) {
      perror("send() failed.");
      exit(EXIT_FAILURE);
    }
    totalrecv = 0;
    int index = 0;
    while (totalrecv < MAX_MSGSIZE) {
      if ((bufrecv = recv(sock, &buf[index], MSGSIZE, 0)) > 0) {
        if (buf[bufrecv - 1] == '\n') {
          buf[bufrecv - 1] = '\0';
          if (strcmp(buf, "quit") == 0) {
            close(sock);
            return EXIT_SUCCESS;
          } else {
            break;
          }
        }
        totalrecv += bufrecv;
      } else if (bufrecv == 0) {
        perror("ERROR_EMPTY_RESPONSE");
        exit(EXIT_FAILURE);
      } else {
        perror("recv() failed.");
        exit(EXIT_FAILURE);
      }
    }
    printf("server return : %s\n\n", buf);
  }
  return EXIT_SUCCESS;
}
