#ifndef MAIN_BOSS_H
#define MAIN_BOSS_H

#define INVINCIBILITY_TIME 12

#include "boss_enemy.h"

class MainBoss : public Boss_Enemy {
  public:
    MainBoss(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY, int hp);

    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill() { hp--; } 
    void setClone(bool cl){ clone = cl;}
    bool isClone(){ return clone;}
    std::vector< Sprite*> projectiles;
  private:
    bool invincible = false;
    double invincibilityTimer = 0.0;
    bool clone = false;

};

#endif
