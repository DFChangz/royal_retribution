#include "character.h"

Character::Character(SDL_Renderer *renderer, SDL_Surface *surf, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  EventHandler *eventHandler)
    : Sprite(renderer, surf, filename, error_handler, width, height, pos_x,
    pos_y) {    

  createListeners(eventHandler);
}

Character::Character(SDL_Renderer *renderer, SDL_Surface *surf, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, EventHandler *eventHandler)
    : Sprite(renderer, surf, filename, error_handler, pos_x,
    pos_y) {

  createListeners(eventHandler);
}

void Character::load() {
  Sprite::load();

  pos_x = WIDTH / 2 - rect.w / 2;
  pos_y = HEIGHT / 2 - rect.h / 2;
}

void Character::createListeners(EventHandler *eventHandler) {
  eventHandler->addListener(SDL_KEYDOWN, [&]() { velocityX = SPEED_CHAR; }, SDLK_d);
  eventHandler->addListener(SDL_KEYDOWN, [&]() { velocityX = -SPEED_CHAR; }, SDLK_a);
  eventHandler->addListener(SDL_KEYDOWN, [&]() { velocityY = SPEED_CHAR; }, SDLK_s);
  eventHandler->addListener(SDL_KEYDOWN, [&]() { velocityY = -SPEED_CHAR; }, SDLK_w);

  eventHandler->addListener(SDL_KEYUP, [&]() { velocityX = 0; }, SDLK_d);
  eventHandler->addListener(SDL_KEYUP, [&]() { velocityX = 0; }, SDLK_a);
  eventHandler->addListener(SDL_KEYUP, [&]() { velocityY = 0; }, SDLK_s);
  eventHandler->addListener(SDL_KEYUP, [&]() { velocityY = 0; }, SDLK_w);
  
  eventHandler->addListener(SDL_KEYDOWN, [&]() { speedMultiplier = 4; }, SDLK_LSHIFT);
  eventHandler->addListener(SDL_KEYUP, [&]() { speedMultiplier = 1; }, SDLK_LSHIFT);
}
