#ifndef PICKUP_H
#define PICKUP_H

#include "sprite.h"

class Pickup : public Sprite {
  public:
    Pickup(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, bool collidable, bool powerup_p);

    Pickup(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int pos_x, int pos_y, bool collidable, bool powerup);

    bool isPickedUp() { return pickedUp; }
    void pickUp() { pickedUp = true; }
    bool isActivated(){ return activated;}
    void activate(){ activated = true;}
    virtual bool isPickup(){ return true; }
    bool isPowerup() { return powerup; }

  private:
    bool pickedUp = false;
    bool powerup = false;
    bool activated = false;


};

#endif
