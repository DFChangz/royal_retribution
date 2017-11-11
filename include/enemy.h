#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_FPS             5
#define ENEMY_IDLE_FRAMES     1
#define ENEMY_MOVING_FRAMES   4

#define ENEMY_DIE_POS         44
#define ENEMY_R_MOVING_POS    28
#define ENEMY_L_MOVING_POS    24
#define ENEMY_UP_MOVING_POS   20
#define ENEMY_DOWN_MOVING_POS 16

#include "sprite.h"
#include "constants.h"
#include "event_handler.h"
#include "types.h"
#include "util.h"

//Header file for enemy class that holds enemy position and its rect
class Enemy : public Sprite {
  public:
    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY);

    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int pos_x, int pos_y, double velocityX, double velocityY);

    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);

    void thaw();
    void freeze();
    void kill() { dead = true; } 
    bool isDead() { return dead; }
    bool isEnemy() { return true; }

    std::string dir="right";

    void followWhenClose(Image* sprite, double radius);
    bool checkDistance(Image* sprite, double radius);
    void followSprite();
    virtual doubleRect getDoubleRect();
  protected:
    Image* shouldFollow = nullptr;

    bool dead = false;
    bool frozen = false;
    bool exploded = false;
    bool following = false;
    bool flipXVelocity = false;
    bool flipYVelocity = false;

    double radiusFollow;
    double tempVX = 150;
    double tempVY = 150;
    double explodingTimer = 0;

  private:
    void attemptFollow();
    void idleAnimation(double);
};

#endif
