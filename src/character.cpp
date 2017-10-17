/*
 * character.cpp
 */

#include "state.h"
#include "character.h"
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
  attackingTimer += seconds;
  staSec += seconds;


  // update stamina
  updateSta();

  // make it so stam always increases
  if (!running && sta < 1) {
    sta += 0.01;
  }

  if (lastAttack && attackingTimer
      > 1/(CHARACTER_FPS*speedMultiplier)*ATTACK_FRAMES) {
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
 
  for(Pickup* item : inventory){
    if(item != nullptr && !item->isActivated())
      item->render(camera);
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
  if(static_cast<Sprite*>(image)->isTrap()){
    static_cast<Sprite*>(image)->pair->setCollidable(true);
    state->collisionDetector.updateBuckets(static_cast<Sprite*>(image)->pair, state->map);
    
    SDL_SetTextureAlphaMod(static_cast<Sprite*>(image)->pair->getTexture(), 255);
    static_cast<Sprite*>(image)->setCollidable(false);
    this->setPair(static_cast<Sprite*>(image)->pair);
    return;
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

  //When collision detector detects a collision play the sound effect
  if (image->isEnemy() && (!attacking || collisionDir != dir) && !invincible) {
    audioHandler->play("collision", 1);

    hearts--;

    invincibilitySeconds = 0;
    /*if attacking an enemy that collides in the same direction the enemy dies
    you get 1000 points and if you are paired with a door, it opens
    */
  } else if (attacking && collisionDir == dir && image->isEnemy()) {
    audioHandler->play("kill", 1);
    static_cast<Enemy*>(image)->kill();
    if(this->pair != nullptr){
      this->pair->setCollidable(false);
      SDL_SetTextureAlphaMod(this->pair->getTexture(), 0);
      this->pair = nullptr;
    }
    state->engine->score += 1000;
    
  }

  Sprite::notifyCollision(image, intersection);
}

void Character::updateSta() {
  // if running
  if (running) {
    if (velocityX != 0 || velocityY != 0) {
      if (sta > 0) {
        sta -= 0.05;
      } else {
        speedMultiplier = 1;
        running = false;
      }
    }
  // if not running
  } else {
    if (sta < 1 && sta > 0) {
      sta += 0.005;
    }
  }
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
    speedMultiplier = 4; running = true; }, SDLK_LSHIFT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    speedMultiplier = 1; running = false; }, SDLK_LSHIFT);
  //object interaction 
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    interacting = true;}, SDLK_e);
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
