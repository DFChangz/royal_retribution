#include "hand.h"

Hand::Hand(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, double velocityX,
    double velocityY)
    : Enemy(renderer, filename, error_handler, width, height, pos_x,
    pos_y, velocityX, velocityY) {

    rect.h /= 2;
    rect.w /= 2;

}

doubleRect Hand::getDoubleRect(){
    doubleRect x;
    x.x = pos_x;
    x.y = pos_y;
    x.w = rect.w;
    x.h = rect.h;
    return x;
}
void Hand::update(double seconds){
    if(!dead){collidable =true;}
    if(!inAir){
        groundTime += seconds;
        velocityX = 0;
        velocityY = 0;
        if(groundTime > STATIONARY_TIME){
            groundTime = 0;
        }
    } else{
        Sprite::update(seconds);
    }
}
void Hand::notifyCollision(Image* img, doubleRect* intersection, bool resolved){
    if(!img->isCharacter()){
        inAir = true;
        return;
    }
    Enemy::notifyCollision(img, intersection, resolved);
}
