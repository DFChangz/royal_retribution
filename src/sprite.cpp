#include "sprite.h"
//Definitions for the the parent class that other sprite objects inherit from

/*Constructor that takes creates that sets makes a sprite from a inputed image
file along with a given position to load in the sprite and size of it's rect*/
Sprite::Sprite(SDL_Renderer *renderer, std::string image_filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  bool collidable_p)
  : Image(renderer, image_filename, error_handler) {

  setPosition(pos_x, pos_y);

  collidable = collidable_p;

  srcRect = {0, 0, width, height};
  rect = {(int) pos_x, (int) pos_y, width, height};
}
//Another constructor, but instead sets the width and height of the rect to 0
Sprite::Sprite(SDL_Renderer *renderer, std::string image_filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, bool collidable_p)
  : Image(renderer, image_filename, error_handler) {

  setPosition(pos_x, pos_y);

  collidable = collidable_p;

  srcRect = {0, 0, 0, 0};
  rect = {(int) pos_x, (int) pos_y, 0, 0};
}


void Sprite::load() {
  Image::load();
  /*When width and height were not specified, the rect is set to the the 
  width and height of the image texture that was loaded*/
  if (rect.w == 0 && rect.h == 0) {
    get_texture_size(texture, &(rect.w), &(rect.h));
  }
}

//moves the position of sprite based on current velocity and time passed
void Sprite::update(double seconds) {
  int new_x = speedMultiplier * velocityX * seconds + pos_x;
  int new_y = speedMultiplier * velocityY * seconds + pos_y;

  setPosition(new_x, new_y);
}

void Sprite::animate(double seconds, int start_frame, int end_frame, int fps) {
  double spf = 1/((double) fps);
  if (currentFrame > end_frame || currentFrame < start_frame) {
    currentFrame = start_frame - 1;
    timer = spf;
  }

  if (timer >= spf)  {
    currentFrame = ((currentFrame - start_frame + 1) % (end_frame - start_frame + 1)) + start_frame;
    int row = currentFrame / 5;
    int col = currentFrame % 5;

    srcRect.x = col*(srcRect.w + SPRITE_PADDING_AMOUNT);
    srcRect.y = row*(srcRect.h + SPRITE_PADDING_AMOUNT);
    
    timer = 0;
  }
  timer += seconds;
}

void Sprite::render() {
  int renderResult = 0;
  if (srcRect.w == 0 && srcRect.h == 0) {
    renderResult = SDL_RenderCopy(renderer, texture, NULL, &rect);
  } else {
    renderResult = SDL_RenderCopy(renderer, texture, &srcRect, &rect);
  }

  if (renderResult < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

SDL_Rect* Sprite::getDestRect() {
  return &rect;
}

bool Sprite::isCollidable() {
  return collidable;
}

void Sprite::get_texture_size(SDL_Texture *texture, int *width, int *height) {
  if (SDL_QueryTexture(texture, NULL, NULL, width, height) < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

/*if the intersection of 2 collidable sprites have more height overlap, y 
velocity changes direction if more width, x direction changes*/
void Sprite::notifyCollision(Image*, SDL_Rect* intersection) {
  if (intersection->w > intersection->h) {
    velocityY *= -1;
  } else {
    velocityX *= -1;
  }
}

Sprite::~Sprite() {

}

void Sprite::setPosition(int x, int y) {
  pos_x = x;
  pos_y = y;
  
  rect.x = (int) x;
  rect.y = (int) y;
}
