/*
 * collision_detector.h
 */

#include "collision_detector.h"
//Definitions of the collision detector class

/* two iterators that go through the whole vector of images comparing one image
to all the others */ 
void CollisionDetector::check(std::vector<Image*>* images) {
  for (auto it = images->begin(); it != images->end(); ++it) {
    for (auto it2 = images->begin(); it2 != images->end(); ++it2) {
      if (it == it2) continue;
      /* when both object are collidable see if their rects touch if so
      relay to that image that the collision happened */
      if ((*it)->isCollidable() && (*it2)->isCollidable() && (*it)->getDestRect() != nullptr &&
          (*it2)->getDestRect() != nullptr) {

        SDL_Rect intersection;
        if (SDL_IntersectRect((*it)->getDestRect(), (*it2)->getDestRect(),
          &intersection) == SDL_TRUE) {

          (*it)->notifyCollision(*it2, &intersection);
        }
      }
    }
  }
}
