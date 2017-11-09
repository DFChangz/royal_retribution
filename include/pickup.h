#ifndef PICKUP_H
#define PICKUP_H

#include "sprite.h"
enum item_type {keyNum, coinNum, foodNum};

class Pickup : public Sprite {
  public:
    Pickup(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, bool collidable, bool powerup_p, int theType);

    Pickup(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int pos_x, int pos_y, bool collidable, bool powerup, int theType);

    bool isPickedUp() { return pickedUp; }
    void pickUp();
    bool isActivated(){ return activated;}
    void activate();
    virtual bool isPickup(){ return true; }
    bool isPowerup() { return powerup; }
    int getType(){ return type; }

    void onPickUp(int);
    void updateList();
    static int inventoryPos;
  private:
    bool pickedUp = false;
    bool powerup = false;
    bool activated = false;
    int type = keyNum;
};

#endif
