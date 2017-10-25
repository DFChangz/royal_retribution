#include "pickup.h"
#include "character.h"

Pickup::Pickup(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
  int width, int height, int pos_x, int pos_y, bool collidable, bool powerup, int theType):
  Sprite(renderer, filename, error_handler, width, height, pos_x, pos_y, collidable),
  powerup(powerup), type(theType){
  

}

Pickup::Pickup(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
  int pos_x, int pos_y, bool collidable, bool powerup, int theType):
  Sprite(renderer, filename, error_handler, pos_x, pos_y, collidable),
  powerup(powerup), type(theType){

}

void Pickup::pickUp() {
  pickedUp = true;
  if(powerup){
    Character::activePowerups.push_back(type);
  }
}

