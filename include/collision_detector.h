#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <iostream>
#include <vector>
#include "image.h"

/*The header file for the collision class that holds a vector of all images
on screen and checks when they are intersecting in position.*/
class CollisionDetector {
  public:
    CollisionDetector(std::vector<Image*>*);
    void check();
  private:
    std::vector<Image*>* images;
};
#endif
