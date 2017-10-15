#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <iostream>
#include <vector>
#include "image.h"
#include "map.h"

#define BUCKET_ROWS 10
#define BUCKET_COLS 10

/*The header file for the collision class that holds a vector of all images
on screen and checks when they are intersecting in position.*/
class Map;
class CollisionDetector {
  public:
    CollisionDetector();

    void check(std::vector<Image*>*, Map* map = nullptr);

    void updateBuckets(Image* image, Map* map = nullptr);
  private:
    void checkInBuckets();
    void checkCollision(Image*, Image*);
    std::vector<collision_bucket> buckets;
    void initializeBuckets();
};
#endif
