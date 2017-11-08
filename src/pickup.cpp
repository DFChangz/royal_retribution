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

void Pickup::onPickUp(int) {
  pickedUp = true;
  setFixed(true);
  setPosition(inventoryPos * 40, 66);
  SDL_SetTextureAlphaMod(texture, 255);
  if(!powerup){
    inventoryPos++;
  }
}

void Pickup::activate() {
  activated = true;

  SDL_SetTextureAlphaMod(texture, 0);
}

int Pickup::inventoryPos = 0;
