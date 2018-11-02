#include "socket.h"

void Socket::set_listenfd() {
  this->listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->listenfd < 0) {
    std::cout << "socket() failed." << std::endl;
    exit(1);
  }
}
void Socket::set_sockaddr_in() {
  memset(&this->serv_addr, 0, sizeof(this->serv_addr));
  this->serv_addr.sin_family      = AF_INET;
  this->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  this->serv_addr.sin_port        = htons(this->port);
}
int Socket::set_socket() {
  this->set_listenfd();
  int optival = 1;
  if (setsockopt(this->listenfd, SOL_SOCKET, SO_REUSEADDR, &optival,
                 sizeof(optival)) < 0) {
    std::cout << "setsockopt() NO_REUSEADDR failed." << std::endl;
    close(listenfd);
    return -1;
  }
  if (setsockopt(this->listenfd, SOL_SOCKET, SO_NOSIGPIPE, &optival,
                 sizeof(optival)) < 0) {
    std::cout << "setsockopt() NO_SIGPIPE failed." << std::endl;
    close(listenfd);
    return -1;
  }
  this->set_sockaddr_in();
  if (bind(this->listenfd, (const sockaddr*)&this->serv_addr,
           sizeof(this->serv_addr)) < 0) {
    std::perror("bind() failed.");
    close(this->listenfd);
    return -1;
  }
  if (listen(this->listenfd, SOMAXCONN) < 0) {
    std::perror("listen() failed.");
    close(this->listenfd);
    return -1;
  }

  return 0;
}
