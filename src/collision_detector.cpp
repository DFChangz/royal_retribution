#include "collision_detector.h"
//Definitions of the collision detector class

//Constructor takes in a vector of images in the game
CollisionDetector::CollisionDetector(std::vector<Image*>* images) : images(images) {}

void CollisionDetector::check() {
  /*to iterators that go through the whole vector of images comparing one image
  to all the others*/ 
  for (auto it = images->begin(); it != images->end(); ++it) {
    for (auto it2 = images->begin(); it2 != images->end(); ++it2) {
      if (it == it2) continue;
      /* when both object are collidable see if their rects touch if so
      relay to that image that the collision happened */
      if ((*it)->isCollidable() && (*it2)->isCollidable() && (*it)->getRect() != nullptr &&
          (*it2)->getRect() != nullptr) {

        SDL_Rect intersection;
        if (SDL_IntersectRect((*it)->getRect(), (*it2)->getRect(),
          &intersection) == SDL_TRUE) {

          (*it)->notifyCollision(*it2, &intersection);
        }
      }
    }
  }
}
