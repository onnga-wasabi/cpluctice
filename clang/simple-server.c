#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int rsock, wsock;
  struct sockaddr_in addr, client;
  unsigned int len;
  int ret;

  rsock = socket(AF_INET, SOCK_STREAM, 0);

  if (rsock < 0) {
    fprintf(stderr, "Error. Cannot make socket\n");
    return -1;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;

  ret = bind(rsock, (struct sockaddr *)&addr, sizeof(addr));

  if (ret < 0) {
    fprintf(stderr, "Error. Cannot make socket\n");
    return -1;
  }

  while (1) {
    listen(rsock, 5);
    len = sizeof(client);
    wsock = accept(rsock, (struct sockaddr *)&client, &len);
    if (wsock < 0)
      break;
    write(wsock, "HTTP1.1 200 OK", 14);
    close(wsock);
  }
}
