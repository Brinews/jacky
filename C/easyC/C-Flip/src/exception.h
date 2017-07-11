#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>

class PpmArrayInputFormatException {
  std::string s;
public:
  PpmArrayInputFormatException(std::string);
  std::string getErrorMessage();
};

#endif