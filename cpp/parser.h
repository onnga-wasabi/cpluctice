#ifndef _parser_h_
#define _parser_h_

#include <iostream>
#include <vector>

class HTTP_PARSER {
public:
  static std::vector<std::string> split(std::string str, const char literal);
  static std::string getFilename(std::string filename);
  static std::string loadFile(std::string header);
};

#endif
