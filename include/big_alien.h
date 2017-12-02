#ifndef BIG_ALIEN_H
#define BIG_ALIEN_H

#define WAIT_TIME 5.0

#include "boss_enemy.h"
#include "hand.h"


class BigAlien : public Boss_Enemy {
  public:
    BigAlien(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, int hp, Sprite* king_p);
    ~BigAlien();

    virtual doubleRect getDoubleRect();
    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill() { return; } 
    virtual void render(Camera*, double interpol_alpha = 1);

    bool setHead(Sprite*);
    bool setHands(Hand*, Hand*);
    std::vector< Enemy*> minions;
    bool isDying() { return dying; }

  private:
    void attackWith(Hand* hand, double seconds);

    Sprite *king = nullptr;
    Sprite *head = nullptr;
    Hand *leftHand = nullptr;
    Hand *rightHand = nullptr;

    int fade = 255;
    int targetX = 0;
    int targetY = 0;
    double timePassed = 0.0;

    bool left = false;
    bool dying = false;
    bool settingUp = false;
    bool attacking = false;
    bool returning = false;
};

#endif
