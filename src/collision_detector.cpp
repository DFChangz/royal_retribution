/*
 * collision_detector.h
 */

#include "collision_detector.h"
#include "character.h"
//Definitions of the collision detector class

/* two iterators that go through the whole vector of images comparing one image
to all the others */ 
void CollisionDetector::check(std::map<std::string,Image*>* images, Map* map) {
  std::cout << "starts coll det\n";
  for (it = images->begin(); it != images->end(); ++it) {
    std::cout << "starts loop\n";
    if (!it->second->isCollidable()) continue;

    for (auto it2 = images->begin(); it2 != images->end(); ++it2) {
      if (it == it2) continue;
      checkCollision(it->second, it2->second);
    }

    if (map != nullptr) {   
      for (auto it2 = map->tiles.begin();
          it2 != map->tiles.end(); ++it2) {

        if (!it2->image->isCollidable())
          break;

        checkCollision(it->second, it2->image);
      }
      for (auto it2 = map->additions.begin();
          it2 != map->additions.end(); ++it2) {

        checkCollision(it->second, it2->image);
      }
    }
  }
}

/* when both object are collidable see if their rects touch if so
relay to that image that the collision happened */
void CollisionDetector::checkCollision(Image* img1, Image* img2) {
  if (img1->isCollidable() && img2->isCollidable() && img1->getDestRect() != nullptr &&
      img2->getDestRect() != nullptr) {

    SDL_Rect intersection;
    if (SDL_IntersectRect(img1->getDestRect(), img2->getDestRect(),
      &intersection) == SDL_TRUE) {

        img1->notifyCollision(img2, &intersection);
    }
  }
}
