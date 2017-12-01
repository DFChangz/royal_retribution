#ifndef HANDS_H
#define HANDS_H

#define STATIONARY_TIME	2.0
#include "enemy.h"

class Hand : public Enemy {
  public:
    Hand(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY);

    virtual doubleRect getDoubleRect();
    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill() { wasAttacked = true; } 
    virtual bool isHand() { return true; }
   
    void killed(){dead = true;}
    bool attacked(){return wasAttacked;}
    void nullAttacked(){wasAttacked = false;}

  private:
    bool wasAttacked = false;
    bool inAir = false;
    double groundTime = 0.0;
};

#endif
