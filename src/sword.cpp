/*
 * sword.cpp
 */

#include "sword.h"
#include "state.h"
#include "character.h"

// constructor that takes in arguments for the width and height for the rect
Sword::Sword(SDL_Renderer *renderer, std::string filename, ErrorHandler
  *errorHandler, int w, int h, int pos_x, int pos_y, Sprite *king_p,
  EventHandler *eventHandler, Audio *audioHandler, State *state_p)
    : Sprite(renderer, filename, errorHandler, w, h, pos_x, pos_y, true),
    audioHandler(audioHandler) {

  rect.w *= 2;
  rect.h *= 2;

  king = king_p;
  state = state_p;
  createListeners(eventHandler);
}

void Sword::update(double seconds) {
  attackingTimer += seconds;

  pos_x = king->pos_x - 40;
  pos_y = king->pos_y - 31;
  dir = static_cast<Character*>(king)->dir;
  
  if (lastAttack
      && attackingTimer > ATTACK_FRAMES/(CHARACTER_FPS*2.0)) {
    attacking = false;
    lastAttack = false;
  }

  Sprite::update(seconds);

  if (attacking) {
    switch (dir[0]) {
      case 'u':
        Sprite::animate(seconds, U_SWORD_POS, U_SWORD_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*2.0);
        break;
      case 'd':
        Sprite::animate(seconds, D_SWORD_POS, D_SWORD_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*2.0);
        break;
      case 'r':
        Sprite::animate(seconds, R_SWORD_POS, R_SWORD_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*2.0);
        break;
      case 'l':
        Sprite::animate(seconds, L_SWORD_POS, L_SWORD_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*2.0);
        break;
    }
  }
}

void Sword::render(Camera* camera, double interpol_alpha) {
  if (attacking) {
    SDL_SetTextureAlphaMod(texture, 255);
  } else {
    SDL_SetTextureAlphaMod(texture, 0);
  }
  Sprite::render(camera, interpol_alpha);
}

void Sword::notifyCollision(Image* image, doubleRect*, bool) {
  if (attacking && image->isEnemy()) {
    //check ul
    if (image->pos_x+32 > pos_x+20 && image->pos_x < pos_x+56
        && image->pos_y+50 > pos_y+20 && image->pos_y < pos_y+56) { ul = true; }
    // check ur
    if (image->pos_x > pos_x+56 && image->pos_x < pos_x+92
        && image->pos_y+50 > pos_y+20 && image->pos_y < pos_y+56) { ur = true; }
    // check dl
    if (image->pos_x+32 > pos_x+20 && image->pos_x < pos_x+56
        && image->pos_y > pos_y+56 && image->pos_y < pos_y+92) { dl = true; }
    // check dr
    if (image->pos_x > pos_x+56 && image->pos_x < pos_x+92
        && image->pos_y > pos_y+56 && image->pos_y < pos_y+92) { dr = true; }
  }
  // enemies die when attacked
  if (attacking && image->isEnemy() && !static_cast<Enemy*>(image)->isDead()) {
    switch (dir[0]) {
      case 'u':
        if (ul || ur) kill(image);
        break;
      case 'l':
        if (ul || dl) kill(image);
        break;
      case 'd':
        if (dl || dr) kill(image);
        break;
      case 'r':
        if (ur || dr) kill(image);
        break;
    }
    ul = false; ur = false; dl = false; dr = false;
  }
}

void Sword::kill(Image* image) {
  static_cast<Enemy*>(image)->kill();
  audioHandler->play("kill", 0);
  if (this->king->pair != nullptr) {
    this->king->pair->setCollidable(false);
    //SDL_SetTextureAlphaMod(this->king->pair->getTexture(), 0);
    this->king->pair = nullptr;
    state->deactivateInstructionText();
  }
  static_cast<Character*>(king)->updateExp();
}

void Sword::createListeners(EventHandler *eventHandler) {
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!static_cast<Character*>(king)->frozen)
      attacking = true;}, SDLK_SPACE);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    lastAttack = true; attackingTimer = 0;}, SDLK_SPACE);
}

void Sword::cleanup() {}

Sword::~Sword() {}
