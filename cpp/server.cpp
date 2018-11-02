#include <string.h>

#include "socket.h"

#include "config.h"
#include "parser.h"
#include "response.h"

int http_server() {
  Socket socket(HTTP_PORT);
  socket.set_socket();
  char buf[BUF_SIZE];

  int client = -1;

  while (1) {
    if ((client = accept(socket.get_listenfd(), (struct sockaddr*)NULL, NULL)) <
        0) {
      std::perror("accept() failed.");
      exit(1);
    }

    std::string header = "";
    memset(buf, 0, sizeof(buf));

    int offset  = 0;
    int recvlen = 0;

    for (;;) {
      recvlen = recv(client, buf, BUF_SIZE - 1, 0);

      if (recvlen > 0) {
        header += buf;
        offset = header.size() - HEADER_SUFFIX.size();
        if (header.compare(offset, HEADER_SUFFIX.size(), HEADER_SUFFIX) == 0)
          break;

      } else if (recvlen == 0) {
        close(client);
        std::cout << client << "close sita" << std::endl;
        break;

      } else {  // recvlen < 0
        std::perror("recv() failed.");
        close(client);
        exit(1);
      }
    }
    if (header == std::string("")) continue;

    std::string file_name = HTTP_PARSER::getFilename(header);
    std::string response  = HTTP_RESPONSE::createResponse(file_name);

    if (send(client, response.c_str(), response.length(), 0) < 0) {
      std::perror("send() failed.");
      close(client);
      exit(1);
    }
    close(client);
  }
  close(socket.get_listenfd());
  return 0;
}

int main() {
  http_server();
  return 0;
}
