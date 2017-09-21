#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "error_handler.h"

/*Header file for the base class image which handles an image and makes it 
into a texture to be rendered by the game engine also hold a collision detector
and its coordinates*/
class Image {
  public:
    virtual void load();
    virtual void update(double seconds);
    virtual void render();
    virtual void cleanup();

    virtual void notifyCollision(Image*, SDL_Rect*) {};

    virtual bool isCollidable();
    virtual SDL_Rect* getDestRect();

    Image(SDL_Renderer *renderer, std::string,
      ErrorHandler *error_handler);

    virtual ~Image();

    virtual bool isEnemy() { return false; }
  protected:
    SDL_Renderer *renderer;
    ErrorHandler *error_handler;
    const std::string image_file;
    SDL_Texture *texture = nullptr;
  private:
    SDL_Surface *surf;
};
#endif
