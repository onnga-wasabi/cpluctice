#ifndef _parser_h_
#define _parser_h_

#include <iostream>
#include <vector>

class HTTP_PARSER {
public:
  static std::vector<std::string> split(std::string str, const char literal);
  static std::string get_filename(std::string filename);
  static std::string load_file(std::string header);
};

#endif
