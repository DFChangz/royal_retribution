/*
 * collision_detector.h
 */

#include <algorithm>
#include "collision_detector.h"
#include "character.h"
#include "types.h"
//Definitions of the collision detector class

CollisionDetector::CollisionDetector() {
  initializeBuckets();
}

/* two iterators that go through the whole vector of images comparing one image
to all the others */ 
void CollisionDetector::check(Camera* camera, Map* map) {
  checkInBuckets(camera, map);

  /*for (auto it = images->begin(); it != images->end(); ++it) {
    if (!(*it)->isCollidable()) continue;

    if (map != nullptr) {   
      for (auto it2 = map->tiles.begin();
          it2 != map->tiles.end(); ++it2) {

        if (!it2->image->isCollidable())
          break;

        checkCollision(*it, it2->image);
      }
      for (auto it2 = map->additions.begin();
          it2 != map->additions.end(); ++it2) {

        if (!it2->image->isCollidable())
          break;

        checkCollision(*it, it2->image);
      }
    }
  }*/
}

void CollisionDetector::checkInBuckets(Camera* camera, Map* map) {
  for (auto bucket_it = buckets.begin(); bucket_it != buckets.end();
    bucket_it++) {

    if (map != nullptr) {
      int bucket_index = bucket_it - buckets.begin();
      const int GRID_WIDTH = map->width / BUCKET_COLS;
      const int GRID_HEIGHT = map->height / BUCKET_ROWS;
      SDL_Rect bucket_rect = {bucket_index % BUCKET_COLS * GRID_WIDTH,
        bucket_index / BUCKET_COLS * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};

      SDL_Rect camera_rect = camera->getRect();

      if (!SDL_HasIntersection(&bucket_rect, &camera_rect)) continue;
    }

    for (auto img1_it = bucket_it->begin(); img1_it != bucket_it->end();
      img1_it++) {
      for (auto img2_it = bucket_it->begin(); img2_it != bucket_it->end();
        img2_it++) {
        if (img1_it == img2_it) continue;

        checkCollision(*img1_it, *img2_it);
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

// Updates the buckets for the specified image
//void CollisionDetector::updateBuckets(Image* image, int img_index) {
void CollisionDetector::updateBuckets(Image* image, Map* map) {
  unsigned int width;
  unsigned int height;
  if (map == nullptr) {
    width = WIDTH;
    height = HEIGHT;
  } else {
    width = map->width;
    height = map->height;
  }

  if (!image->isCollidable()) return;

  int x = image->pos_x;
  int y = image->pos_y;

  const int GRID_WIDTH = width / BUCKET_COLS;
  const int GRID_HEIGHT = height / BUCKET_ROWS;

  // Add image to all potential buckets
  unsigned int grid_x = x / GRID_WIDTH;
  unsigned int grid_y = y / GRID_HEIGHT;
  SDL_Rect grid_rect = {0, 0, GRID_WIDTH, GRID_HEIGHT};
  SDL_Rect* image_rect = image->getDestRect();

  for (auto bucket = image->buckets.begin(); bucket != image->buckets.end();
    ) {
    grid_rect.x = *bucket % BUCKET_COLS * GRID_WIDTH;
    grid_rect.y = *bucket / BUCKET_COLS * GRID_HEIGHT;

    if (!SDL_HasIntersection(&grid_rect, image_rect)) {
      auto img_bucket = &(buckets[*bucket]);
      img_bucket->erase(std::find((*img_bucket).begin(), (*img_bucket).end(), image));
      bucket = image->buckets.erase(bucket);
    } else {
      bucket++;
    }
  }

  grid_rect.x = grid_x * GRID_WIDTH;
  grid_rect.y = grid_y * GRID_HEIGHT;

  for (int i = grid_x; i < BUCKET_COLS &&
      SDL_HasIntersection(&grid_rect, image_rect); i++) {

    for (int j = grid_y; j < BUCKET_ROWS &&
        SDL_HasIntersection(&grid_rect, image_rect); j++) {

      unsigned int index = j * BUCKET_COLS + i;

      grid_rect.x = (i + 1) * GRID_WIDTH;
      grid_rect.y = (j + 1) * GRID_HEIGHT;

      if (std::find(image->buckets.begin(), image->buckets.end(), index) !=
         image->buckets.end())
        continue;

      buckets[index].push_back(image);
      image->buckets.push_back(index);
    }

    grid_rect.y = grid_y * GRID_HEIGHT;
  }
}

void CollisionDetector::initializeBuckets() {
  for (int i = 0; i < BUCKET_ROWS*BUCKET_COLS; i++) {
    buckets.push_back(collision_bucket());
  }
}
