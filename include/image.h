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

    virtual void notifyCollision(Image*, SDL_Rect*) {};

    virtual bool isCollidable();
    virtual SDL_Rect* getRect();

    Image(SDL_Renderer *renderer, std::string,
      ErrorHandler *error_handler);

    virtual ~Image();

    virtual bool isEnemy() { return false; }
  protected:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    ErrorHandler *error_handler;
    const std::string image_file;
  private:
    SDL_Surface *surf;
};
#endif
