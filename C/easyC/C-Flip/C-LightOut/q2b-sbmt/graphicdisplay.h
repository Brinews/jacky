#ifndef __GRAPHICDISPLAY_H__
#define __GRAPHICDISPLAY_H__

#include <iostream>
#include <vector>
#include "observer.h"

#include "window.h"

class Cell;

class GraphicDisplay: public Observer {
  std::vector<std::vector<char>> theDisplay;
  const int gridSize; //you may or may not need to use this
 public:
  GraphicDisplay(int n);

  void notify(Subject &whoNotified) override;
  SubscriptionType subType() const override;

  ~GraphicDisplay();

  void display(Xwindow *win, int w, int h);
};

#endif
