#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "parser.h"

std::vector<std::string> HTTP_PARSER::split(std::string str,
                                            const char literal) {
  std::vector<std::string> v;
  std::string buf = "";
  for (const char c : str) {
    if (c == literal) {
      v.push_back(buf);
      buf = "";
    } else {
      buf += c;
    }
  }
  if (buf != "") v.push_back(buf);
  return v;
}

std::string HTTP_PARSER::loadFile(std::string filename) {
  std::ifstream ifs(filename);
  std::cout << "requested to " << filename << std::endl;
  if (ifs.fail()) {
    std::cout << "load " << filename << " failed." << std::endl;
    std::cout << std::endl;
    return FILE_NOT_FOUND;
  }
  std::cout << std::endl;
  std::istreambuf_iterator<char> it(ifs);
  std::istreambuf_iterator<char> last;
  std::string str(it, last);
  return str;
}

std::string HTTP_PARSER::getFilename(std::string header) {
  std::vector<std::string> lines  = HTTP_PARSER::split(header, '\n');
  std::vector<std::string> blocks = HTTP_PARSER::split(lines[0], ' ');
  return blocks[1];
}
