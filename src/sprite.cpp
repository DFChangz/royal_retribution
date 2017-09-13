#include "sprite.h"

Sprite::Sprite(SDL_Renderer *renderer, SDL_Surface *surf, 
  std::string image_filename, ErrorHandler *error_handler, int width, 
  int height, int pos_x, int pos_y)
  : Image(renderer, surf, image_filename, error_handler) {

  rect = {pos_x, pos_y, width, height};
}

Sprite::Sprite(SDL_Renderer *renderer, SDL_Surface *surf, 
  std::string image_filename, ErrorHandler *error_handler, int pos_x, 
  int pos_y)
  : Image(renderer, surf, image_filename, error_handler), pos_x(pos_x),
    pos_y(pos_y) {

  rect = {pos_x, pos_y, 0, 0};
}

Sprite::~Sprite() {
  cleanup();
}

void Sprite::load() {
  Image::load();

  int width = rect.w;
  int height = rect.h;
  if (rect.w == 0 && rect.h == 0) {
    get_texture_size(texture, &width, &height);
  }

  rect = {rect.x, rect.y, width, height};
}

void Sprite::update(double seconds) {
  pos_x = speedMultiplier * velocityX * seconds + pos_x;
  pos_y = speedMultiplier * velocityY * seconds + pos_y;

  if (pos_x < 0) pos_x = 0;
  else if (pos_x + rect.w > WIDTH) pos_x = WIDTH - rect.w;
  else if (pos_y < 0) pos_y = 0;
  else if (pos_y + rect.h > HEIGHT) pos_y = HEIGHT - rect.h;

  rect = {(int) pos_x, (int) pos_y, rect.w, rect.h};
}

void Sprite::render() {
  if (SDL_RenderCopy(renderer, texture, NULL, &rect) < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

void Sprite::get_texture_size(SDL_Texture *texture, int *width, int *height) {
  if (SDL_QueryTexture(texture, NULL, NULL, width, height) < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}
