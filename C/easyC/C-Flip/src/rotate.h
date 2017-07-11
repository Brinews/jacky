#ifndef ROTATE_H
#define ROTATE_H

#include "image.h"

class RotateImage: public Image {
    Image *img;
public:
  RotateImage(Image *im){ img = im; }
  void setSource(std::istream *inp) override;
  void render(PpmArray &ppm) override;
  ~RotateImage() { delete img; }
};
#endif
