#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_FPS             5
#define ENEMY_MOVING_FRAMES   4
#define ENEMY_IDLE_FRAMES     1
#define ENEMY_R_MOVING_POS    5
#define ENEMY_L_MOVING_POS    0
#define ENEMY_UP_IDLE_POS     12
#define ENEMY_DOWN_IDLE_POS   11

#include "sprite.h"
#include "event_handler.h"
#include "constants.h"

//Header file for enemy class that holds enemy position and its rect
class Enemy : public Sprite {
  public:
    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY);

    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int pos_x, int pos_y, double velocityX, double velocityY);

    virtual void update(double seconds);

    bool isEnemy() {return true;}
  private:
    std::string dir="right";
    void idleAnimation(double);
};

#endif
