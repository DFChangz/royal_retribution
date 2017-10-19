/*
 * sprite.cpp
 */

#include "sprite.h"
#include "character.h"
//Definitions for the the parent class that other sprite objects inherit from

/*Constructor that makes a sprite from an inputed image file along with a gi-
ven position to load in the sprite and size of its rect*/
Sprite::Sprite(SDL_Renderer *renderer, std::string image_filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  bool collidable_p, bool fixed_p)
  : Image(renderer, image_filename, error_handler) {

  fixed = fixed_p;

  setPosition(pos_x, pos_y);

  collidable = collidable_p;

  srcRect = {0, 0, width, height};
  rect = {(int) pos_x, (int) pos_y, width, height};
}

//Another constructor, but instead sets the width and height of the rect to 0
Sprite::Sprite(SDL_Renderer *renderer, std::string image_filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, bool collidable_p,
    bool fixed_p)
  : Image(renderer, image_filename, error_handler) {

  fixed = fixed_p;

  setPosition(pos_x, pos_y);

  collidable = collidable_p;

  srcRect = {0, 0, 0, 0};
  rect = {(int) pos_x, (int) pos_y, 0, 0};
}

void Sprite::load(SDL_Texture *texture_p) {
  Image::load(texture_p);
  /*When width and height were not specified, the rect is set to the the 
  width and height of the image texture that was loaded*/
  if (rect.w == 0 && rect.h == 0) {
    get_texture_size(texture, &(rect.w), &(rect.h));
  }
  
  if(door && !collidable){
    SDL_SetTextureAlphaMod(this->getTexture(), 0);   
  }
}

//moves the position of sprite based on current velocity and time passed
void Sprite::update(double seconds) {
  double new_x = speedMultiplier * velocityX * seconds + pos_x;
  double new_y = speedMultiplier * velocityY * seconds + pos_y;

  setPosition(new_x, new_y);
}

void Sprite::animate(double seconds, int start_frame, int end_frame, int fps) {
  animate(seconds, start_frame, end_frame, -1, -1, fps);
}
void Sprite::animate(double seconds, int start_frame, int end_frame,
  int frame_width, int frame_height, int fps) {

  if (start_frame > end_frame) end_frame = start_frame;

  int padding_x = SPRITE_PADDING_AMOUNT_X;
  int padding_y = SPRITE_PADDING_AMOUNT_Y;

  if (usualSrcRect.w != 0 && usualSrcRect.h != 0 && frame_width == -1
      && frame_height == -1) {
    srcRect = usualSrcRect;
    usualSrcRect = {0, 0, 0, 0};

    rect.w = 2*srcRect.w;
    rect.h = 2*srcRect.h;
  }

  if (frame_width != -1 && frame_height != -1) {
    if (usualSrcRect.w == 0 && usualSrcRect.h == 0) usualSrcRect = srcRect;
    srcRect = {srcRect.x, srcRect.y, frame_width, frame_height};
    
    padding_x -= (srcRect.w - usualSrcRect.w);
    padding_y -= (srcRect.h - usualSrcRect.h);

    rect.w = 2*srcRect.w;
    rect.h = 2*srcRect.h;
  }

  double spf = 1/((double) fps);
  if (currentFrame > end_frame || currentFrame < start_frame) {
    currentFrame = start_frame - 1;
    timer = spf;
  }

  if (timer >= spf)  {
    currentFrame = ((currentFrame - start_frame + 1)
      % (end_frame - start_frame + 1)) + start_frame;
  
    int row = currentFrame / NUM_ROWS;
    int col = currentFrame % NUM_COLS;

    srcRect.x = col*(srcRect.w + padding_x);
    srcRect.y = row*(srcRect.h + padding_y);
    if(chest && currentFrame == 63){
      pair = this;
      return;
    }
    
    timer = 0;
  }
  timer += seconds;
}

void Sprite::render(Camera* camera) {
  if (camera == nullptr) {
    error_handler->quit(__func__, "must specify a non-null camera to render for a sprite!"); 
  }

  int renderResult = 0;
  if (srcRect.w == 0 && srcRect.h == 0) {
    renderResult = camera->render(renderer, texture, NULL, &rect, fixed);
  } else {
    renderResult = camera->render(renderer, texture, &srcRect, &rect, fixed);
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

void Sprite::setCollidable(bool collide){
  collidable = collide;
}

void Sprite::get_texture_size(SDL_Texture *texture, int *width, int *height) {
  if (SDL_QueryTexture(texture, NULL, NULL, width, height) < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

/*if the intersection of 2 collidable sprites have more height overlap, y 
velocity changes direction if more width, x direction changes*/
void Sprite::notifyCollision(Image* img, SDL_Rect* intersection) {

  if (pair != nullptr){
    pair->collidable = true;
    SDL_SetTextureAlphaMod(pair->getTexture(), 255);   
  }
  if (abs(velocityY) > abs(velocityX)) {
    if (velocityY != 0) {
      double velRatio = (double)abs(velocityY) / ((double) abs(velocityY) + (double) abs(img->velocityY));

      pos_y -= velocityY / abs(velocityY) * ((intersection->h) * velRatio);
    }
  } else if (abs(velocityY) == abs(velocityX)) {
    if (intersection->w > intersection->h) {
      if (velocityY != 0) {
        double velRatio = (double)abs(velocityY) / ((double) abs(velocityY) + (double) abs(img->velocityY));

        pos_y -= velocityY / abs(velocityY) * ((intersection->h) * velRatio);
      }
    } else {
      if (velocityX != 0) {
        double velRatio = abs(velocityX) / (abs(velocityX) + abs(img->velocityX));
        pos_x -= velocityX / abs(velocityX) * ((intersection->w)* velRatio);
      }
    }
  } else {
    if (velocityX != 0) {
      double velRatio = abs(velocityX) / (abs(velocityX) + abs(img->velocityX));
      pos_x -= velocityX / abs(velocityX) * ((intersection->w)* velRatio);
    }
  }

  setPosition(pos_x, pos_y);
}

Sprite::~Sprite() {

}

void Sprite::setSrcRect(int posX, int posY, int rectWidth, int rectHeight){
  srcRect.x = posX;
  srcRect.y = posY;
  srcRect.w = rectWidth;
  srcRect.h = rectHeight;
}
void Sprite::setPosition(double x, double y) {
  pos_x = x;
  pos_y = y;
  
  rect.x = (int) x;
  rect.y = (int) y;
}
