#include <string.h>

#include "socket.h"

#include "config.h"
#include "parser.h"
#include "response.h"

void sig_child(int sig) {
  wait(&sig);
}

int http_server() {
  Socket socket(HTTP_PORT);
  socket.set_socket();
  struct timeval tv;
  char buf[BUF_SIZE];

  int client = -1;

  for (;;) {
    if ((client = accept(socket.get_listenfd(), (struct sockaddr*)NULL, NULL)) <
        0) {
      std::perror("accept() failed.");
      exit(1);
    }
    pid_t pid;
    if ((pid = fork()) < 0) {
      std::perror("fork() failed.");
      exit(1);

    } else if (pid == 0) {
      close(socket.get_listenfd());

      std::string header = "";
      memset(buf, 0, sizeof(buf));

      int offset  = 0;
      int recvlen = 0;

      for (;;) {
        recvlen = recv(client, buf, BUF_SIZE - 1, 0);

        if (recvlen < 0) {
          std::perror("recv() failed.");
          std::cout << pid << std::endl;
          close(client);
          _exit(1);

        } else if (recvlen == 0) {
          close(client);
          client = -1;
          _exit(0);

        } else {
          header += buf;
          offset = header.size() - HEADER_SUFFIX.size();
          if (header.compare(offset, HEADER_SUFFIX.size(), HEADER_SUFFIX) == 0)
            break;
        }
      }

      std::string file_name = HTTP_PARSER::getFilename(header);
      std::string response  = HTTP_RESPONSE::createResponse(file_name);

      if (send(client, response.c_str(), response.length(), 0) < 0) {
        std::perror("send() failed.");
        close(client);
        _exit(1);
      }
      close(client);
      client = -1;
      _exit(0);

    } else {
      close(client);
      client = -1;
    }
  }
  close(socket.get_listenfd());
  return 0;
}

int main() {
  signal(SIGCHLD, sig_child);
  http_server();
  return 0;
}
