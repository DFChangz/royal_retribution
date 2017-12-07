#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "sprite.h"

class Projectile : public Sprite {
  public:
    Projectile(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double radius, bool enem, 
      Sprite *owner = nullptr);

    void goTo(Sprite *dest);
    void goTo(double x, double y);
    void throwThis();
    bool wasThrown(){return thrown;}
    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect* intersection, bool resolved = false);
    virtual bool isEnemy(){ return enemy;}
    
    bool frozen = false;
  private:
    double tempY = 0.0;
    double tempX = 0.0;
    double destX = 0.0;
    double destY = 0.0;
    double throwPosX = 0.0;
    double throwPosY = 0.0;
    Sprite *thrower;
    bool thrown = false;
    double dissipateRadius;
     bool enemy = true;

};

#endif
