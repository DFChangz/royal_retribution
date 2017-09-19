#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <iostream>
#include <vector>
#include "image.h"

/*The header file for the collision class that holds a vector of all images
on screen and checks when they are intersecting in position.*/
class CollisionDetector {
  public:
    void check(std::vector<Image*>*);
};
#endif
