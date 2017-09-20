#include "character.h"
//Definitions for the character class that inherits from Sprite

//constructor that takes in arguments for the width and height for the rect
Character::Character(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  EventHandler *eventHandler, Audio *audioHandler, int sheetFrames)
    : Sprite(renderer, filename, error_handler, width, height, pos_x, pos_y, sheetFrames),
    audioHandler(audioHandler) {    

  createListeners(eventHandler);
}

//constructor that does not take in width or height for the rect
Character::Character(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, EventHandler *eventHandler,
  Audio *audioHandler)
    : Sprite(renderer, filename, error_handler, pos_x, pos_y),
    audioHandler(audioHandler) {

  createListeners(eventHandler);
}

void Character::load() {
  Sprite::load();

  pos_x = WIDTH / 2 - rect.w / 2;
  pos_y = HEIGHT / 2 - rect.h / 2;
}

void Character::update(double seconds) {
  Sprite::update(seconds);

  //if the character would leave the window, stop movement in that direction.
  if (pos_x < 0) pos_x = 0;
  else if (pos_x + rect.w > WIDTH) pos_x = WIDTH - rect.w;

  if (pos_y < 0) pos_y = 0;
  else if (pos_y + rect.h > HEIGHT) pos_y = HEIGHT - rect.h;

  rect.x = (int) pos_x;
  rect.y = (int) pos_y;
}

void Character::notifyCollision(Image* image, SDL_Rect* intersection) {
  Sprite::notifyCollision(image, intersection);

  //When collision detector detects a collision play the sound effect
  if (image->isEnemy()) {
    audioHandler->play("collision", 1);
  }
}

void Character::createListeners(EventHandler *eventHandler) {
  /*when wasd keys are pressed down the velocities are set to move in the 
  specified direction: W = up; S = down; A = left; D = right*/
  eventHandler->addListener(SDL_KEYDOWN, [&]() { velocityX = SPEED_CHAR; }, SDLK_d);
  eventHandler->addListener(SDL_KEYDOWN, [&]() { velocityX = -SPEED_CHAR; }, SDLK_a);
  eventHandler->addListener(SDL_KEYDOWN, [&]() { velocityY = SPEED_CHAR; }, SDLK_s);
  eventHandler->addListener(SDL_KEYDOWN, [&]() { velocityY = -SPEED_CHAR; }, SDLK_w);

  //when key is released, velocity set back to 0
  eventHandler->addListener(SDL_KEYUP, [&]() { velocityX = 0; }, SDLK_d);
  eventHandler->addListener(SDL_KEYUP, [&]() { velocityX = 0; }, SDLK_a);
  eventHandler->addListener(SDL_KEYUP, [&]() { velocityY = 0; }, SDLK_s);
  eventHandler->addListener(SDL_KEYUP, [&]() { velocityY = 0; }, SDLK_w);
  
  // BOOST FOR DEBUGGING PURPOSES
  eventHandler->addListener(SDL_KEYDOWN, [&]() { speedMultiplier = 4; }, SDLK_LSHIFT);
  eventHandler->addListener(SDL_KEYUP, [&]() { speedMultiplier = 1; }, SDLK_LSHIFT);
}
