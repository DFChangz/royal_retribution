#include "image.h"
//Definitions for the the parent class that any image object inherit from

void Image::load() {
  //Makes a surface where the texture is created from
  surf = IMG_Load(image_file.c_str());
  if (surf == nullptr) {
    error_handler->quit(__func__, IMG_GetError());
  }

  texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (texture == nullptr) {
    SDL_FreeSurface(surf);
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

//Destroys the texture to free up memory
void Image::cleanup() {
  if (texture != nullptr) {
    SDL_DestroyTexture(texture);
  }
}

SDL_Rect* Image::getDestRect() {
  error_handler->quit(__func__, "Image does not have a rect. This only works on subclasses");
  return NULL;
}

bool Image::isCollidable() {
  return false;
}


Image::Image(SDL_Renderer *renderer_p, std::string image_file,
  ErrorHandler *error_handler_p) :
    image_file(image_file) {
    
    renderer = renderer_p;
    error_handler = error_handler_p;
}

Image::~Image() {
  cleanup();
}
