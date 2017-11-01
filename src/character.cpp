/*
 * character.cpp
 */

#include "character.h"
#include "state.h"
#include "playing_state.h"
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

  startingX = pos_x;
  startingY = pos_y;

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

  startingX = pos_x;
  startingY = pos_y;

  state = state_p;
  
  createListeners(eventHandler);
}

void Character::update(double seconds) {
  invincibilitySeconds += seconds;
  attackingTimer += seconds;
  staSec += seconds;

  std::cout << "charX: " << pos_x << "\n";
  std::cout << "charY: " << pos_y << "\n";

  // update sta
  if (!frozen) updateSta();

  if (lastAttack && attackingTimer > 1/(CHARACTER_FPS)*ATTACK_FRAMES) {
    attacking = false;
    lastAttack = false;
  }

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
      Sprite::animate(seconds, R_RUNNING_POS, R_RUNNING_POS+RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityX < 0) {
      dir = "left";
      Sprite::animate(seconds, L_RUNNING_POS, L_RUNNING_POS+RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityY > 0) {
      dir = "down";
      Sprite::animate(seconds, D_RUNNING_POS, D_RUNNING_POS+RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityY < 0) {
      dir = "up";
      Sprite::animate(seconds, U_RUNNING_POS, U_RUNNING_POS+RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else {
      idleAnimation(seconds);
    }
  }

  if (attacking) {
    if (dir == "up")
      Sprite::animate(seconds, U_ATTACK_POS, U_ATTACK_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS);
    else if (dir == "down")
      Sprite::animate(seconds, D_ATTACK_POS, D_ATTACK_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS);
    else if (dir == "right")
      Sprite::animate(seconds, R_ATTACK_POS, R_ATTACK_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS);
    else if (dir == "left")
      Sprite::animate(seconds, L_ATTACK_POS, L_ATTACK_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS);
  }
  if (Character::currFloor == 2 && Character::highestFloor == 1) {
    Character::highestFloor = 2;
  }
  if (Character::currFloor == 3 && Character::highestFloor == 2) {
    Character::highestFloor = 3;
  }
}

void Character::render(Camera* camera, double interpol_alpha) {
  if (invincibilitySeconds < INVINCIBLE_TIME)
    SDL_SetTextureAlphaMod(texture, 100);
  else {
    if (!falling) SDL_SetTextureAlphaMod(texture, 255);
    hurt = false;
  }

  Sprite::render(camera, interpol_alpha);
 
  for(Pickup* item : inventory){
    if(item != nullptr && !item->isActivated())
      item->render(camera, interpol_alpha);
  }
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
  std::string collisionDir = "";

  /*after stepping on a trap tile the associated door appears and is collidable
  The trap is no longer collidable, and the character is paired with the door
  */
  if(static_cast<Sprite*>(image)->isTrap()&& static_cast<Sprite*>(image)->pair != nullptr){
    static_cast<Sprite*>(image)->pair->setCollidable(true);
    state->collisionDetector.updateBuckets(static_cast<Sprite*>(image)->pair, state->map);

    SDL_SetTextureAlphaMod(static_cast<Sprite*>(image)->pair->getTexture(), 255);
    static_cast<Sprite*>(image)->setCollidable(false);
    this->setPair(static_cast<Sprite*>(image)->pair);

    state->activateInstructionText(trapNum);
    return;
  }
  else if(static_cast<Sprite*>(image)->isDoor() && static_cast<Sprite*>(image)->pair == nullptr){
    if(interacting){
      for(Pickup* item : inventory){
        if( item->getType() == keyNum){
          SDL_SetTextureAlphaMod(image->getTexture(), 0);
          static_cast<Sprite*>(image)->setCollidable(false);
          item->activate();
        }
      }
    }
  }
  if(static_cast<Sprite*>(image)->isHole()){
    PlayingState::fallen = 1;
    falling = true;
    velocityX = 0;
    velocityY = 0;
    //state->activateInstructionText(holeNum);
  }
  // either up/down
  if (intersection->w > intersection->h) {
    if (intersection->y <= image->pos_y)
      collisionDir = "down";
    else
      collisionDir = "up";
  } else {
    if (intersection->x <= image->pos_x)
      collisionDir = "right";
    else
      collisionDir = "left";
  }

  if(static_cast<Sprite*>(image)->isChest() && interacting == true){
      static_cast<Sprite*>(image)->pair = this;
  }

  if(static_cast<Sprite*>(image)->isDoor() && interacting == true){
    for( unsigned i = 0; i < inventory.size(); i++){
      if(!inventory[i]->isActivated()){
        inventory[i]->activate();
      }
    } 
  }
  //When collision detector detects a collision play the sound effect
  if ((image->isEnemy() && (!attacking || collisionDir != dir) && !invincible)
    || (static_cast<Sprite*>(image)->isBlade() && !invincible)) {
    audioHandler->play("collision", 1);

    if (!hurt) {
      hearts--;
      hurt = true;
    }

    invincibilitySeconds = 0;
  }
  if (!image->isSword()) Sprite::notifyCollision(image, intersection);
}

void Character::updateSta() {
  if (sta > 1) sta = 1;
  if (sta < 0) sta = 0;
  // if running
  if (running) {
    if (velocityX != 0 || velocityY != 0) {
      if (sta > 0) {
        sta -= 0.02;
      } else {
        speedMultiplier = 1;
        running = false;
      }
    }
  // if not running
  } else {
    sta += 0.005;
  }
}

void Character::updateExp() {
  exp += expInc;
  if (exp >= 1) {
    exp--;
    level++;
    expInc -= 0.02;
    if (level == 9) expInc = 0.02;
    if (level == 10) expInc = 0;
  }
  state->engine->score += 1000;
}

void Character::createListeners(EventHandler *eventHandler) {
  // w, a, s, d conrols
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityX = SPEED_CHAR; }, SDLK_d);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityX = -SPEED_CHAR; }, SDLK_a);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityY = SPEED_CHAR; }, SDLK_s);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityY = -SPEED_CHAR; }, SDLK_w);
  // up, down, left, right controls
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityX = SPEED_CHAR; }, SDLK_RIGHT);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityX = -SPEED_CHAR; }, SDLK_LEFT);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityY = SPEED_CHAR; }, SDLK_DOWN);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityY = -SPEED_CHAR; }, SDLK_UP);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) attacking = true; }, SDLK_SPACE);

  //when key is released, velocity set back to 0
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityX > 0) velocityX = 0;}, SDLK_d);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityX < 0) velocityX = 0;}, SDLK_a);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityY > 0) velocityY = 0;}, SDLK_s);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityY < 0) velocityY = 0;}, SDLK_w);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityX > 0) velocityX = 0;}, SDLK_RIGHT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityX < 0) velocityX = 0;}, SDLK_LEFT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityY > 0) velocityY = 0;}, SDLK_DOWN);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityY < 0) velocityY = 0;}, SDLK_UP);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    lastAttack = true; attackingTimer = 0;}, SDLK_SPACE);
  
  // boost control
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) speedMultiplier = 4; running = true; }, SDLK_LSHIFT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    speedMultiplier = 1; running = false; }, SDLK_LSHIFT);
  //object interaction 
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) interacting = true;}, SDLK_e);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    interacting = false;}, SDLK_e);
}



void Character::cleanup(){
  /*for(Sprite* item : inventory){
    if(item != nullptr){
      delete item;
      item = nullptr; 
    }
  }*/
}

Character::~Character(){
//  cleanup();

}

std::vector<Pickup*> Character::inventory;

std::vector<int>Character::activePowerups;

int Character::hearts = 6;

int Character::level = 1;

double Character::exp = 0;

int Character::currFloor = 1;

int Character::highestFloor = 1;
