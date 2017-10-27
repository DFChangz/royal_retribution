#include "camera.h"
#include "character.h"

Camera::Camera(ErrorHandler *errorHandler_p) : character(nullptr) {
  errorHandler = errorHandler_p;
}

void Camera::setCharacter(Character* character_p) {
  character = character_p;
}

void Camera::updatePosition() {
  if (character != nullptr) {
    prevRect = getRect();

    SDL_Rect* char_rect = character->getDestRect();
    pos_x = character->pos_x - WIDTH / 2 + char_rect->w/2;
    pos_y = character->pos_y - HEIGHT / 2 + char_rect->h/2;
  }
}

SDL_Rect Camera::getRect() {
  SDL_Rect cameraRect = {(int) pos_x, (int) pos_y, WIDTH, HEIGHT};
  return cameraRect;
}

int Camera::render(SDL_Renderer* renderer, SDL_Texture* texture,
  SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Rect* img_prevRect, double alpha, 
  bool fixed) {

  SDL_Rect cameraRect = getRect();
  
  /* LOOK OVER THIS FOR LATER */
  //interpolate(&prevRect, &cameraRect, alpha);
  //interpolate(img_prevRect, destRect, alpha);

  if (SDL_HasIntersection(destRect, &cameraRect) || fixed) {
    if (!fixed) {
      destRect->x -= pos_x;
      destRect->y -= pos_y;
    }

    return SDL_RenderCopy(renderer, texture, srcRect, destRect);
  } else {
    return 0;
  }
}

int Camera::render(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect, bool fixed) {
  return render(renderer, texture, srcRect, destRect, NULL, 0, fixed);
}

void Camera::interpolate(SDL_Rect* orig_rect, SDL_Rect* new_rect, double alpha) {
  if (orig_rect != NULL && new_rect != NULL) {
    new_rect->x += (new_rect->x - orig_rect->x) * alpha;
    new_rect->y += (new_rect->y - orig_rect->y) * alpha;
  }
}
