#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <string.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "config.h"

class HTTP_RESPONSE {
public:
  static std::string status200(std::string filename);
};

#endif
