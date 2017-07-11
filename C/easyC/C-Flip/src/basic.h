#ifndef BASIC_H
#define BASIC_H

#include "image.h"

class BasicImage: public Image {
  std::istream *source;
public:
  BasicImage(){}
  void setSource(std::istream *inp) override;
  void render(PpmArray &ppm) override;
};
#endif