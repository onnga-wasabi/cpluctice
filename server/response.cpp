#include <string.h>
#include <iostream>

#include "config.h"
#include "parser.h"
#include "response.h"

std::string HTTP_RESPONSE::createResponse(std::string filename) {
  std::string body = HTTP_PARSER::loadFile(DOCUMENT_ROOT + filename);
  if (body == FILE_NOT_FOUND) {
    body = filename.substr(1) + " " + FILE_NOT_FOUND;
    return HTTP_RESPONSE::status404(body);
  }
  return HTTP_RESPONSE::status200(body);
}

std::string HTTP_RESPONSE::status200(std::string body) {
  std::string header =
      "HTTP/1.1 200 OK\n"
      "content-type: text/html; charset=UTF-8;\n"
      "connection: close\n"
      "content-length: " +
      std::to_string(body.length()) + HEADER_SUFFIX;
  return header + body;
}

std::string HTTP_RESPONSE::status404(std::string body) {
  std::string header =
      "HTTP/1.1 404 Not Found\n"
      "content-type: text/html; charset=UTF-8;\n"
      "content-length: " +
      std::to_string(body.length()) +
      "\n"
      "connection: close" +
      HEADER_SUFFIX;
  return header + body;
}

/*
std::string HTTP_RESPONSE::status404(std::string filename) {
  std::string header =
      "HTTP/1.1 404 Not Found\n"
      "conten-type: text/html; charset=UTF-8;\n" +
      HEADER_SUFFIX;
  return header + FILE_NOT_FOUND;
}
*/
