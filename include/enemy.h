#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"
#include "event_handler.h"
#include "constants.h"

class Enemy : public Sprite {
  public:
    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY);

    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int pos_x, int pos_y, double velocityX, double velocityY);

    virtual void update(double seconds);

    bool isEnemy() {return true;}
};

#endif
