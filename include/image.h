#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "error_handler.h"

class Image {
  public:
    virtual void load();
    virtual void update(double seconds);
    virtual void render();
    virtual void cleanup();

    Image(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler);

    virtual ~Image();
  protected:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    ErrorHandler *error_handler;
  private:
    const std::string image_file;
    SDL_Surface *surf;
};
#endif
