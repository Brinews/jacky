#ifndef SEPIA_H
#define SEPIA_H

#include "image.h"

class SepiaImage: public Image {
  Image *img;
public:
  SepiaImage(Image *im){ img = im; }
  void setSource(std::istream *inp) override;
  void render(PpmArray &ppm) override;
  ~SepiaImage() { delete img; }
};
#endif
