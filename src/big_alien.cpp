#include "big_alien.h"

BigAlien::BigAlien(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, double velocityX,
    double velocityY, int health)
    : Boss_Enemy(renderer, filename, error_handler, width, height, pos_x,
    pos_y, velocityX, velocityY, health) {

    rect.h /= 2;
    rect.w /= 2;

    timePassed = 0;

}

bool BigAlien::setHands(Hand *rh, Hand *lh){
    rightHand = rh;
    if(rightHand == nullptr) {return false;}
    leftHand = lh;

    if(leftHand == nullptr) {return false;}

    return true;
}
bool BigAlien::setHead(Sprite* h){
    head = h;
    if(head == nullptr) {return false;}

    return true;
}
doubleRect BigAlien::getDoubleRect(){
    doubleRect x;
    x.x = pos_x;
    x.y = head->pos_y;
    x.w = rect.w;
    x.h = rect.h + head->getDoubleRect().h;
    return x;
}
void BigAlien::update(double seconds){
    if(!dead){
        collidable =true;
        if(!frozen){timePassed += seconds;}
    } else { 
        timePassed = 0;
        SDL_SetTextureAlphaMod(head->getTexture(), 0);
        rightHand->killed();  
        leftHand->killed();  
    }
    

    if(timePassed > .5){
        leftHand->setPosition(pos_x + 160, pos_y);
        rightHand->setPosition(pos_x -240, pos_y + 160);
        if(hp % 3 == 0){ 
            rightHand->setPosition(pos_x -240, pos_y + 320);
        } 
    }
    if(timePassed > 2.0){
        leftHand->setPosition(pos_x + 160, pos_y);
        rightHand->setPosition(pos_x, pos_y + 160); 
        if(hp % 3 == 0){ 
            rightHand->setPosition(pos_x, pos_y + 320);
        } 
    }
    if(timePassed > 3.0){
        leftHand->setPosition(pos_x + 160, pos_y);
        rightHand->setPosition(pos_x -160, pos_y); 
    }
    if(timePassed > 3.5){
        leftHand->setPosition(pos_x + 240, pos_y + 160);
        rightHand->setPosition(pos_x -160, pos_y); 
        if(hp % 3 == 0){ 
            leftHand->setPosition(pos_x +240, pos_y + 320);
        } 
    }
    if(timePassed > 5.0){
        leftHand->setPosition(pos_x, pos_y + 160);
        rightHand->setPosition(pos_x -160, pos_y); 
        if(hp % 3 == 0){ 
            leftHand->setPosition(pos_x, pos_y + 320);
        } 
    }
    if(timePassed > 6.0){
        leftHand->setPosition(pos_x + 160, pos_y);
        rightHand->setPosition(pos_x -160, pos_y);
        timePassed = 0.0; 
    }

    if(leftHand->attacked()){
        leftHand->nullAttacked();
        leftHand->setPosition(pos_x + 160, pos_y);
        hp--;
        timePassed = 0;
        std::cout << hp << std::endl;
    }
    if(rightHand->attacked()){
        rightHand->nullAttacked();
        rightHand->setPosition(pos_x -160, pos_y );
        hp--;
        timePassed = 3.0;
        std::cout << hp << std::endl;
    }
    Boss_Enemy::update(seconds);
    //if(hp <= 0){dead = true;}

}
void BigAlien::notifyCollision(Image* img, doubleRect* intersection, bool resolved){
    if(!img->isCharacter()){ return;}
    Enemy::notifyCollision(img, intersection, resolved);
}
BigAlien::~BigAlien(){
/*
    if(head != nullptr){
        delete head;
    }
*/
}
void BigAlien::render(Camera* camera, double interpol_alpha) {
    if(dead && head != nullptr){
        SDL_SetTextureAlphaMod(head->getTexture(), 0);
        return;
    }
    Sprite::render(camera, interpol_alpha);
    //head->render(camera, interpol_alpha);

}
