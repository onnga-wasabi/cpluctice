#ifndef _socket_h_
#define _socket_h_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "config.h"

class Socket {
  int listenfd;
  int port;
  struct sockaddr_in serv_addr;

  void set_option();
  void set_listenfd();
  void start_bind();
  void start_listen();

public:
  // explicit 引数の暗黙な変換をキャンセル
  //「コンストラクタ　初期値　{}」
  explicit Socket(int port_) : port(port_) {}
  void set_sockaddr_in();
  void set_socket();
  static void set_keepalive(int socket);

  // constつけるとメンバ変数の変更，それに関する関数の呼び出しを禁止する
  int get_listenfd() const { return this->listenfd; }
};

#endif
