#include "enemy.h"
//Definitions for the enemy class that inherits from Sprite

//constructor that takes in width and height for the rect
Enemy::Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  double velocity_X, double velocity_Y)
    : Sprite(renderer, filename, error_handler, width, height, pos_x,
    pos_y) {
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

  //if an enemy hits a boundary it goes in the opposite direction
  if (pos_x < 0 || pos_x + rect.w > WIDTH) {
    velocityX *= -1;
  }
  if (pos_y < 0 || pos_y + rect.h > HEIGHT) {
    velocityY *= -1;
  }
}
