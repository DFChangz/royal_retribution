/*
 * character.cpp
 */

#include "character.h"
#include "state.h"
//Definitions for the character class that inherits from Sprite

//constructor that takes in arguments for the width and height for the rect
Character::Character(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  EventHandler *eventHandler, Audio *audioHandler, State *state_p)
    : Sprite(renderer, filename, error_handler, width, height, pos_x, pos_y,
      true),
    audioHandler(audioHandler) {    

  rect.w *= 2;
  rect.h *= 2;

  state = state_p;

  createListeners(eventHandler);
}

//constructor that does not take in width or height for the rect
Character::Character(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, EventHandler *eventHandler,
  Audio *audioHandler, State *state_p)
    : Sprite(renderer, filename, error_handler, pos_x, pos_y,
      true),
    audioHandler(audioHandler) {

  rect.w *= 2;
  rect.h *= 2;

  state = state_p;

  createListeners(eventHandler);
}

void Character::update(double seconds) {
  invincibilitySeconds += seconds;
  invincible = (invincibilitySeconds < INVINCIBLE_TIME) ? true : false;

  Sprite::update(seconds);

  // If attacking, don't move. Hack to get around changing the velocity
  //irreversibly
  if (attacking) {
    pos_x -= velocityX * seconds * speedMultiplier;
    pos_y -= velocityY * seconds * speedMultiplier;
  }

  //if the character would leave the window, stop movement in that direction.
  rect.x = (int) pos_x;
  rect.y = (int) pos_y;

  if (!attacking) {
    if (velocityX > 0) {
      dir = "right";

      Sprite::animate(seconds, R_RUNNING_POS, R_RUNNING_POS + RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityX < 0) {
      dir = "left";

      Sprite::animate(seconds, L_RUNNING_POS, L_RUNNING_POS + RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityY > 0) {
      dir = "down";

      Sprite::animate(seconds, D_RUNNING_POS, D_RUNNING_POS + RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityY < 0) {
      dir = "up";

      Sprite::animate(seconds, U_RUNNING_POS, U_RUNNING_POS + RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else {
      idleAnimation(seconds);
    }
  }

  if (attacking) {
    if (dir == "up")
      Sprite::animate(seconds, U_ATTACK_POS, U_ATTACK_POS + ATTACK_FRAMES - 1,
        36, 36, CHARACTER_FPS*speedMultiplier);
    else if (dir == "down")
      Sprite::animate(seconds, D_ATTACK_POS, D_ATTACK_POS + ATTACK_FRAMES - 1,
        36, 36, CHARACTER_FPS*speedMultiplier);
    else if (dir == "right")
      Sprite::animate(seconds, R_ATTACK_POS, R_ATTACK_POS + ATTACK_FRAMES - 1,
        36, 36, CHARACTER_FPS*speedMultiplier);
    else if (dir == "left")
      Sprite::animate(seconds, L_ATTACK_POS, L_ATTACK_POS + ATTACK_FRAMES - 1,
        36, 36, CHARACTER_FPS*speedMultiplier);
  }
}

void Character::render(Camera* camera) {
  if (invincibilitySeconds < INVINCIBLE_TIME)
    SDL_SetTextureAlphaMod(texture, 100);
  else
    SDL_SetTextureAlphaMod(texture, 255);

  Sprite::render(camera);
}

void Character::idleAnimation(double seconds) {
  int pos = -1;

  if (dir == "right") pos = R_RUNNING_POS;
  else if (dir == "left") pos = L_RUNNING_POS;
  else if (dir == "up") pos = U_RUNNING_POS;
  else if (dir == "down") pos = D_RUNNING_POS;
  else error_handler->quit(__func__, "direction not found");

  Sprite::animate(seconds, pos, pos + IDLE_FRAMES - 1);
}

void Character::notifyCollision(Image* image, SDL_Rect* intersection) {
  //When collision detector detects a collision play the sound effect
  if (image->isEnemy() && !attacking && !invincible) {
    audioHandler->play("collision", 1);

    hearts--;

    invincibilitySeconds = 0;
  } else if (attacking) {
    audioHandler->play("kill", 1);
    static_cast<Enemy*>(image)->kill();
    state->engine->score += 1000;
  }

  Sprite::notifyCollision(image, intersection);
}

void Character::createListeners(EventHandler *eventHandler) {
  // w, a, s, d conrols
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    velocityX = SPEED_CHAR; }, SDLK_d);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    velocityX = -SPEED_CHAR; }, SDLK_a);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    velocityY = SPEED_CHAR; }, SDLK_s);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    velocityY = -SPEED_CHAR; }, SDLK_w);
  // up, down, left, right controls
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    velocityX = SPEED_CHAR; }, SDLK_RIGHT);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    velocityX = -SPEED_CHAR; }, SDLK_LEFT);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    velocityY = SPEED_CHAR; }, SDLK_DOWN);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    velocityY = -SPEED_CHAR; }, SDLK_UP);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    attacking = true; }, SDLK_SPACE);

  //when key is released, velocity set back to 0
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    velocityX = 0;}, SDLK_d);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    velocityX = 0;}, SDLK_a);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    velocityY = 0;}, SDLK_s);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    velocityY = 0;}, SDLK_w);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    velocityX = 0;}, SDLK_RIGHT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    velocityX = 0;}, SDLK_LEFT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    velocityY = 0;}, SDLK_DOWN);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    velocityY = 0;}, SDLK_UP);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    attacking = false; }, SDLK_SPACE);
  
  // boost control
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    speedMultiplier = 4; }, SDLK_LSHIFT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    speedMultiplier = 1; }, SDLK_LSHIFT);
}
