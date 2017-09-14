#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <iostream>
#include <vector>
#include "image.h"

class CollisionDetector {
  public:
    CollisionDetector(std::vector<Image*>*);
    void check();
  private:
    std::vector<Image*>* images;
};
#endif
