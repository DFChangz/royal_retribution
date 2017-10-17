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
    SDL_Rect* char_rect = character->getDestRect();
    pos_x = character->pos_x - WIDTH / 2 + char_rect->w/2;
    pos_y = character->pos_y - HEIGHT / 2 + char_rect->h/2;
  }
}

SDL_Rect Camera::getRect() {
  SDL_Rect cameraRect = {(int) pos_x, (int) pos_y, WIDTH, HEIGHT};
  return cameraRect;
}

int Camera::render(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect, bool fixed) {
  SDL_Rect cameraRect = getRect();
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
