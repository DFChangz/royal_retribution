#include "collision_detector.h"

CollisionDetector::CollisionDetector(std::vector<Image*>* images) : images(images) {}

void CollisionDetector::check() {
  for (auto it = images->begin(); it != images->end(); ++it) {
    for (auto it2 = images->begin(); it2 != images->end(); ++it2) {
      if (it == it2) continue;

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
