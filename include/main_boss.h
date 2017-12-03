#ifndef MAIN_BOSS_H
#define MAIN_BOSS_H

#define INVINCIBILITY_TIME 4
#define MAIN_BOSS_IDLE     72

#include "boss_enemy.h"
#include "map.h"

class MainBoss : public Boss_Enemy {
  public:
    MainBoss(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY, int hp, Map*);

    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill(); 
    void killed(){dead = true;}
    void setClone(bool cl){ clone = cl;}
    bool isClone(){ return clone;}
    std::vector< Sprite*> projectiles;
    MainBoss* clone1 = nullptr;
    MainBoss* clone2 = nullptr;
  private:
    bool invincible = false;
    double invincibilityTimer = 0.0;
    bool clone = false;
    bool positionChosen = false;
    void choosePositions();
    Map* map;
    bool chase = false;

};

#endif
