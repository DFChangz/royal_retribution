#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "event_handler.h"
#include "error_handler.h"
#include "types.h"
#include "constants.h"

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
    SDL_Texture* getTexture();

    Image(SDL_Renderer *renderer, std::string,
      ErrorHandler *error_handler);

    virtual ~Image();

    virtual bool isEnemy() { return false; }

    virtual void setPosition(double x, double y);

    void onHover(EventHandler*, std::function<void()>);
    void onClick(EventHandler*, std::function<void()>);

    double pos_x = 0.0;
    double pos_y = 0.0;

    double velocityX = 0.0;
    double velocityY = 0.0;

  protected:
    SDL_Renderer *renderer;
    ErrorHandler *error_handler;
    const std::string image_file;
    SDL_Texture *texture = nullptr;
  private:
    SDL_Surface *surf;

    // Stores the triggers in the heap until cleanup() for listeners like
    // onHover.
    std::map<int, std::function<void()>>* triggers = nullptr;
    int currentTrigger_i = 0;
};
#endif
