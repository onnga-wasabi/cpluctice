#include <fstream>
#include <iostream>
#include <string>

std::string load_file(char* filename) {
  std::ifstream ifs(filename);
  if (ifs.fail()) {
    std::cerr << "failed." << std::endl;
    exit(1);
  }
  std::istreambuf_iterator<char> it(ifs);
  std::istreambuf_iterator<char> last;
  std::string str(it, last);
  std::cout << str << std::endl;
  return str;
}

#include <iostream>
#include <vector>
std::vector<std::string> split_by_literal(std::string str, const char literal) {
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
  return v;
}

int main() {
  auto str  = load_file("index.html");
  auto vecs = split_by_literal(str, '\n');
  for (const std::string line : vecs) {
    std::cout << line << "line" << std::endl;
  }
  return 0;
}
