#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"
#include "event_handler.h"
#include "constants.h"

class Enemy : public Sprite {
  public:
    Enemy(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
      int speed);

    Enemy(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler, int pos_x, int pos_y, int speed);

    virtual void update(double seconds);

    bool isEnemy() {return true;}
};

#endif
