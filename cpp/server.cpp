#include <string.h>

#include "socket.h"

#include "config.h"
#include "parser.h"
#include "response.h"

int http_server() {
  Socket socket(HTTP_PORT);
  socket.set_socket();
  while (1) {
    int client = 0;
    if ((client = accept(socket.get_listenfd(), (struct sockaddr*)NULL, NULL)) <
        0) {
      std::perror("accept() failed.");
      exit(1);
    }
    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    std::string header = "";
    header.compare(0, 0, "");

    int recvlen = 0;
    do {
      if ((recvlen = recv(client, buf, BUF_SIZE - 1, 0)) < 0) {
        std::perror("recv() failed.");
        close(client);
        break;
      }
      header += buf;
      auto offset = header.size() - HEADER_SUFFIX.size();
      if (header.compare(offset, HEADER_SUFFIX.size(), HEADER_SUFFIX) == 0)
        break;
    } while (recvlen > 0);

    auto file_name = HTTP_PARSER::get_filename(header);
    auto response  = HTTP_RESPONSE::status200(file_name);

    if (write(client, response.c_str(), BUF_SIZE) < 0) {
      std::perror("write() failed.");
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
