#include "socket.h"

#if defined(__APPLE__)
#define TCP_KEEPIDLE TCP_KEEPALIVE
#endif

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
void Socket::set_option() {
  int optival;

  optival = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optival,
                 sizeof(optival)) < 0) {
    std::cout << "setsockopt() NO_REUSEADDR failed." << std::endl;
    close(listenfd);
    exit(1);
  }
  optival = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_NOSIGPIPE, &optival,
                 sizeof(optival)) < 0) {
    // 相手がすでにcloseしている時にsendすると強制終了するのをキャンセル
    std::cout << "setsockopt() NO_SIGPIPE failed." << std::endl;
    close(listenfd);
    exit(1);
  }
}
void Socket::start_bind() {
  if (bind(this->listenfd, (const sockaddr*)&this->serv_addr,
           sizeof(this->serv_addr)) < 0) {
    std::perror("bind() failed.");
    close(this->listenfd);
    exit(1);
  }
}
void Socket::start_listen() {
  if (listen(this->listenfd, SOMAXCONN) < 0) {
    std::perror("listen() failed.");
    close(this->listenfd);
    exit(1);
  }
}
void Socket::set_socket() {
  this->set_listenfd();
  this->set_option();
  this->set_sockaddr_in();
  this->start_bind();
  this->start_listen();
}
void Socket::set_keepalive(int socket) {
  int optival = 1;
  int idle    = 30;
  int intvl   = 10;
  int keepcnt = 2;

  setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &optival, sizeof(optival));
  setsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
  setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
  setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
}
