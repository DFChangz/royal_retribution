#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "error_handler.h"

class Image {
  public:
    virtual void load();
    virtual void update();
    virtual void render();
    virtual void cleanup();

    Image(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler);

    virtual ~Image();
  private:
    SDL_Renderer *renderer;
    ErrorHandler *error_handler;
    const std::string image_file;
    SDL_Surface *surf;
    SDL_Texture *texture;
};
#endif
