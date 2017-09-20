#ifndef SPRITE_H
#define SPRITE_H

#include "image.h"
#include "constants.h"

/*header file for the sprite class that inherits from image and is the parent 
class of the 2 moveable object  classes character and enemy. Holds a velocity
a speed multiplier and a way to detect collisions when they occur*/
class Sprite : public Image {
  public:
    double timer = 0;

    Sprite(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, int sheetFrames = 1);

    Sprite(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int pos_x, int pos_y);

    virtual void load();
    virtual void update(double seconds);
    virtual void render();

    virtual void notifyCollision(Image*, SDL_Rect*);

    virtual bool isCollidable();
    virtual SDL_Rect* getDestRect();

    virtual ~Sprite();
  protected:
    double pos_x = 0.0;
    double pos_y = 0.0;
    double velocityX = 0;
    double velocityY = 0;
    double speedMultiplier = 1;
    ErrorHandler *error_handler;
    SDL_Rect rect;
    SDL_Rect srcRect;
    void get_texture_size(SDL_Texture*, int*, int*);
  private:
    bool animating = false;
    void updateVelocities(double seconds);
    int sheetFrames;
};

#endif
