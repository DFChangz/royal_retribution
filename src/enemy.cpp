/*
 * enemy.cpp
 */

#include "enemy.h"
//Definitions for the enemy class that inherits from Sprite

//constructor that takes in width and height for the rect
Enemy::Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  double velocity_X, double velocity_Y)
    : Sprite(renderer, filename, error_handler, width, height, pos_x,
    pos_y) {

    rect.w *= 2;
    rect.h *= 2;

    velocityX = velocity_X;
    velocityY = velocity_Y;
}

//constructor with no width and height arguments
Enemy::Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, double velocity_X,
  double velocity_Y)
    : Sprite(renderer, filename, error_handler, pos_x, pos_y) {

    velocityX = velocity_X;
    velocityY = velocity_Y;
}

void Enemy::update(double seconds) {
  Sprite::update(seconds);

  if (this->dead){
    velocityY = 0;
    velocityX = 0;
    collidable = false;
    SDL_SetTextureAlphaMod(this->getTexture(), 0);
    return;
  }
  //if an enemy hits a boundary it goes in the opposite direction
  if (pos_x < 0 || pos_x + rect.w > WIDTH) {
    velocityX *= -1;
  }
  if (pos_y < 0 || pos_y + rect.h > HEIGHT) {
    velocityY *= -1;
  }

  if (velocityX > 0) {
    dir = "right";
    Sprite::animate(seconds, ENEMY_R_MOVING_POS, ENEMY_R_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityX < 0) {
    dir = "left";
    Sprite::animate(seconds, ENEMY_L_MOVING_POS, ENEMY_L_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityY > 0) {
    dir = "down";
    Sprite::animate(seconds, ENEMY_DOWN_MOVING_POS, ENEMY_DOWN_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityY < 0) {
    dir = "up";
    Sprite::animate(seconds, ENEMY_UP_MOVING_POS, ENEMY_UP_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  }
}

void Enemy::notifyCollision(Image* img, SDL_Rect* intersection) {
  Sprite::notifyCollision(img, intersection);

  if (intersection->w > intersection->h) {
    velocityY *= -1;
  } else {
    velocityX *= -1;
  }
}

void Enemy::idleAnimation(double seconds) {
  int pos = -1;

  if (dir == "right") pos = ENEMY_R_MOVING_POS;
  else if (dir == "left") pos = ENEMY_L_MOVING_POS;
  else if (dir == "up") pos = ENEMY_UP_MOVING_POS;
  else if (dir == "down") pos = ENEMY_DOWN_MOVING_POS;
  else error_handler->quit(__func__, "direction not found");

  Sprite::animate(seconds, pos, pos + ENEMY_IDLE_FRAMES - 1);
}
