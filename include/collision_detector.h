#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <iostream>
#include <vector>
#include "image.h"

class CollisionDetector {
  public:
    void check(std::vector<Image*>*);
};
#endif
