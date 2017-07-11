#ifndef FLIP_H
#define FLIP_H

#include "image.h"

class FlipImage: public Image {
    Image *img;
public:
  FlipImage(Image *im){ img = im; }
  void setSource(std::istream *inp) override;
  void render(PpmArray &ppm) override;
  ~FlipImage() { delete img; }
};
#endif
