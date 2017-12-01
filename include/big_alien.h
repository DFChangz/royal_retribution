#ifndef BIG_ALIEN_H
#define BIG_ALIEN_H

#define WAIT_TIME 5.0

#include "boss_enemy.h"
#include "hand.h"

class BigAlien : public Boss_Enemy {
  public:
    BigAlien(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY, int hp);
    ~BigAlien();

    virtual doubleRect getDoubleRect();
    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill() {return;} 
    virtual void render(Camera*, double interpol_alpha = 1);
    bool setHands(Hand*, Hand*);
    bool setHead(Sprite*);
    std::vector< Enemy*> minions;
  private:
    Hand *rightHand = nullptr;
    Hand *leftHand = nullptr;
    Sprite *head = nullptr;
    double timePassed;

};

#endif
