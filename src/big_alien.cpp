#include "big_alien.h"
#include "character.h"

BigAlien::BigAlien(SDL_Renderer *renderer, std::string filename, ErrorHandler
  *error_handler, int width, int height, int pos_x, int pos_y, int hp,
  Sprite *king_p)
    : Boss_Enemy(renderer, filename, error_handler, width, height,
      pos_x, pos_y, 0, 0, hp) {

  rect.h /= 2;
  rect.w /= 2;

  king = king_p;
}

bool BigAlien::setHands(Hand *rh, Hand *lh){
  leftHand = lh;
  if(leftHand == nullptr) return false;
  rightHand = rh;
  if(rightHand == nullptr) return false;

  return true;
}

bool BigAlien::setHead(Sprite* h){
  head = h;
  if(head == nullptr) return false;

  return true;
}

doubleRect BigAlien::getDoubleRect(){
    doubleRect x;
    x.x = pos_x;
    x.y = pos_y;
    x.w = rect.w;
    x.h = rect.h;
    return x;
}

void BigAlien::update(double seconds){
  if (!fade) {
    dead = true;
    return;
  }
  if (frozen){

    leftHand->velocityY = 0;
    leftHand->velocityX = 0;
    rightHand->velocityY = 0;
    rightHand->velocityX = 0;
    return;
  }
  // if any/both hand(s) are dead
  if (left && static_cast<Hand*>(leftHand)->isDead()) {
    timePassed = 0.0;
    left = false;
  }
  if (!left && static_cast<Hand*>(rightHand)->isDead()) {
    timePassed = 0.0;
    left = true;
  }
  if (static_cast<Hand*>(leftHand)->isDead()
      && static_cast<Hand*>(rightHand)->isDead()) {
    dying = true;
    fade = fadeOut(fade, seconds, 1.0);
    
    return;
  }
  // determine which hand to attack with
  if (left) {
    attackWith(leftHand, seconds);
  } else {
    attackWith(rightHand, seconds);
  }

}

void BigAlien::attackWith(Hand* hand, double seconds) {
  timePassed += seconds;

  // setting up
  if (timePassed < 1.0) {
    settingUp = true;
    hand->velocityY = -20;
  } else {
    // prepare attack
    if (settingUp) {
      attacking = true;
      targetX = king->pos_x + 16;
      targetY = king->pos_y + 25;
    }
  }

  // start attack
  if (attacking) {
    settingUp = false;
    static_cast<Hand*>(hand)->goTo(targetX, targetY);
    if (static_cast<Hand*>(hand)->onGround()) attacking = false;
  }

  // hand is returned
  if (static_cast<Hand*>(hand)->returned()) {
    if (left) left = false;
    else left = true;
    timePassed = 0;
  }
}

void BigAlien::notifyCollision(Image* img, doubleRect*, bool) {
  if (img->isEnemy() || img->isCharacter() || img->isSword()) return;
}

void BigAlien::render(Camera* camera, double interpol_alpha) {
  if (!fade) return;
  Sprite::render(camera, interpol_alpha);
}

BigAlien::~BigAlien(){}
