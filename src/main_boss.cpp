#include "main_boss.h"

MainBoss::MainBoss(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, double velocity_x,
    double velocity_y, int health)
    : Boss_Enemy(renderer, filename, error_handler, width, height, pos_x,
    pos_y, velocity_x, velocity_y, health) {
    tempVX = velocity_x;
    tempVY = velocity_y;
}

void MainBoss::update(double seconds){
    if (dead){
        if(!clone){
            clone1->killed();
            clone2->killed();
        }
        return;
    } else {collidable = true;}

    if(clone){invincible = true;}

    if(phase == 2){
    if (shouldFollow != nullptr)
        attemptFollow();
        Sprite::update(seconds);
    } else if(phase == 1){ 
        velocityX = 0;
        velocityY = 0;
        if(!clone){
            if(!positionChosen){choosePositions();}
            if(wasAttacked){
                invincible = true;
                SDL_SetTextureAlphaMod(getTexture(), 100);
                SDL_SetTextureAlphaMod(clone1->getTexture(), 100);
                SDL_SetTextureAlphaMod(clone2->getTexture(), 100);
                invincibilityTimer += seconds;
                if(invincibilityTimer > INVINCIBILITY_TIME){
                    SDL_SetTextureAlphaMod(getTexture(), 255);
                    SDL_SetTextureAlphaMod(clone1->getTexture(), 255);
                    SDL_SetTextureAlphaMod(clone2->getTexture(), 255);
                    positionChosen = false;
                    wasAttacked = false;
                    clone1->nullAttacked();
                    clone2->nullAttacked();
                    invincibilityTimer = 0;
                    invincible = false;
                    positionChosen = false;
                }
            } else if(clone1->attacked() || clone2->attacked()){

                std::cout << hp<<std::endl;
                SDL_SetTextureAlphaMod(clone1->getTexture(), 0);
                SDL_SetTextureAlphaMod(clone2->getTexture(), 0);
                clone1->setPosition(500, 500);
                clone2->setPosition(500, 500);
                followSprite();
                speedMultiplier = 5;
                Sprite::update(seconds);
                invincible = true;

                if(flipXVelocity || flipYVelocity){
                    positionChosen = false;
                    SDL_SetTextureAlphaMod(clone1->getTexture(), 255);
                    SDL_SetTextureAlphaMod(clone2->getTexture(), 255);
                    clone1->nullAttacked();
                    clone2->nullAttacked();
                    flipXVelocity = false;
                    flipYVelocity = false;
                    invincible = false;
                }
               
            }
        }
    }
    Sprite::animate(seconds, MAIN_BOSS_IDLE, MAIN_BOSS_IDLE
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
    Boss_Enemy::update(seconds);


}
void MainBoss::notifyCollision(Image* img, doubleRect* intersection, bool){
    if(!img->isCharacter()){ return;}
    if (intersection->w > intersection->h) {
        flipYVelocity = true;
    } else {
        flipXVelocity = true;
    }
}
void MainBoss::kill(){
    if(!invincible){
        hp--;
        invincible = true;
    }
    wasAttacked = true;
    flipYVelocity = true;
    flipXVelocity = true;
} 

void MainBoss::choosePositions(){
    int randNum;
    randNum = SDL_GetTicks() % 3;

    switch(randNum){
        case 2:
            setPosition(1094,1074);
            clone1->setPosition(1094, 1219);
            clone2->setPosition(1094, 974);
            positionChosen = true;
            break;
        case 1:
            clone1->setPosition(1094,1074);
            setPosition(1094, 1219);
            clone2->setPosition(1094, 974);
            positionChosen = true;
            break;
        case 0:
            clone2->setPosition(1094,1074);
            clone1->setPosition(1094, 1219);
            setPosition(1094, 974);
            positionChosen = true;
            break;
        default:
            setPosition(800,1074);
            clone1->setPosition(800, 1219);
            clone2->setPosition(800, 974);
            positionChosen = true;
            break;
    }
}
