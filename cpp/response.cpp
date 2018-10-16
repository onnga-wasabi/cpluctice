#include <string.h>
#include <iostream>

#include "config.h"
#include "parser.h"
#include "response.h"

std::string HTTP_RESPONSE::status200(std::string filename) {
  auto body = HTTP_PARSER::load_file(DOCUMENT_ROOT + filename);
  std::string header =
      "HTTP/1.1 200 OK\n"
      "conten-type: text/html; charset=UTF-8;\n"
      "content-length: " +
      std::to_string(body.length()) +
      "\n"
      "connection: keep-alive\n" +
      HEADER_SUFFIX;
  return header + body;
}
