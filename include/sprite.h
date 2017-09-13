#ifndef SPRITE_H
#define SPRITE_H

#include "image.h"
#include "constants.h"

class Sprite : public Image {
  public:
    Sprite(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y);

    Sprite(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler, int pos_x, int pos_y);
    using Image::update;
    virtual void load();
    virtual void update(double seconds);
    virtual void render();

    virtual ~Sprite();
  protected:
    double pos_x = 0.0;
    double pos_y = 0.0;
    double velocityX = 0;
    double velocityY = 0;
    double speedMultiplier = 1;
    ErrorHandler *error_handler;
    SDL_Rect rect;
  private:
    void get_texture_size(SDL_Texture*, int*, int*);
};

#endif
