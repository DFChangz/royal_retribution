/*
 * collision_detector.h
 */

#include <algorithm>
#include "collision_detector.h"
#include "types.h"
#include "character.h"

//Definitions of the collision detector class

CollisionDetector::CollisionDetector() {
  initializeBuckets();
}

void CollisionDetector::check(Camera* camera, Map* map) {
  checkInBuckets(camera, map);
}

/* For each bucket on screen, checks for collisions between all items */
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
      camera_rect.x -= 30;
      camera_rect.y -= 30;
      camera_rect.w += 60;
      camera_rect.h += 60;

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

    doubleRect intersection;
    /*SDL_Rect img1Rect = *(img1->getDestRect());
    img1Rect.x = img1->pos_x;
    img1Rect.y = img1->pos_y;
    SDL_Rect img2Rect = *(img2->getDestRect());
    img2Rect.x = img2->pos_x;
    img2Rect.y = img2->pos_y;*/
    //if (SDL_IntersectRect(&img1Rect, &img2Rect,
    //  &intersection) == SDL_TRUE) {
    if (getIntersection(img1, img2, &intersection)) {
        img1->notifyCollision(img2, &intersection);
    }
  }
}

/* gets the intersection of two doubleRects */
bool CollisionDetector::getIntersection(Image* img1, Image* img2, doubleRect* intersection) {
  double aMin, bMin, aMax, bMax;

  aMin = img1->pos_x;
  aMax = aMin + img1->getDestRect()->w;
  bMin = img2->pos_x;
  bMax = bMin + img2->getDestRect()->w;
  if (bMin > aMin)
    aMin = bMin;
  intersection->x = aMin;
  if (bMax < aMax)
    aMax = bMax;
  intersection->w = aMax - aMin;

  aMin = img1->pos_y;
  aMax = aMin + img1->getDestRect()->h;
  bMin = img2->pos_y;
  bMax = bMin + img2->getDestRect()->h;
  if (bMin > aMin)
    aMin = bMin;
  intersection->y = aMin;
  if (bMax < aMax)
    aMax = bMax;
  intersection->h = aMax - aMin;

  if (intersection->w > 0 && intersection->h > 0) return true;
  return false;
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
    grid_rect.y = *bucket / BUCKET_ROWS * GRID_HEIGHT;
    if (!image->isCollidable() || !SDL_HasIntersection(&grid_rect, image_rect)) {
      auto img_bucket = &(buckets[*bucket]);
      img_bucket->erase(std::find((*img_bucket).begin(), (*img_bucket).end(), image));
      bucket = image->buckets.erase(bucket);
    } else {
      bucket++;
    }
  }

  if (!image->isCollidable()) return;

  grid_rect.x = grid_x * GRID_WIDTH;
  grid_rect.y = grid_y * GRID_HEIGHT;

  for (int i = grid_x; i < BUCKET_COLS &&
      SDL_HasIntersection(&grid_rect, image_rect); i++) {

    for (int j = grid_y; j < BUCKET_ROWS &&
        SDL_HasIntersection(&grid_rect, image_rect); j++) {

      unsigned int index = j * BUCKET_COLS + i;

      grid_rect.y = (j + 1) * GRID_HEIGHT;
      
      if (std::find(image->buckets.begin(), image->buckets.end(), index) !=
         image->buckets.end())
        continue;

      buckets[index].push_back(image);
      image->buckets.push_back(index);
    }

    grid_rect.x = (i + 1) * GRID_WIDTH;
    grid_rect.y = grid_y * GRID_HEIGHT;
  }

}

void CollisionDetector::initializeBuckets() {
  for (int i = 0; i < BUCKET_ROWS*BUCKET_COLS; i++) {
    buckets.push_back(collision_bucket());
  }
}
