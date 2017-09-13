#include "image.h"

void Image::load() {
  surf = IMG_Load(image_file.c_str());
  if (surf == nullptr) {
    error_handler->quit(__func__, IMG_GetError());
  }

  texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (texture == nullptr) {
    error_handler->quit(__func__, SDL_GetError());
  }

  SDL_FreeSurface(surf);
}

void Image::update(double) {

}

void Image::render() {
  if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

void Image::cleanup() {
  if (texture != nullptr) {
    SDL_DestroyTexture(texture);
  }
}

Image::Image(SDL_Renderer *renderer, SDL_Surface* surf, std::string image_file,
  ErrorHandler *error_handler) :
    renderer(renderer),
    error_handler(error_handler),
    image_file(image_file),
    surf(surf) {}

Image::~Image() {
  cleanup();
}
