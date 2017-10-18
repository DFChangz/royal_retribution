#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <iostream>
#include <vector>
#include "image.h"
#include "map.h"

/*The header file for the collision class that holds a vector of all images
on screen and checks when they are intersecting in position.*/
class CollisionDetector {
  public:
    void check(std::map<std::string,Image*>*, Map* map = nullptr);

  private:
    void checkCollision(Image*, Image*);
    std::map<std::string,Image*>::iterator it;
};
#endif
