#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "ppmArray.h"

class Image {
public:
  virtual void setSource(std::istream *inp) = 0;
  virtual void render(PpmArray &ppm) = 0;
  virtual ~Image();
};

#endif