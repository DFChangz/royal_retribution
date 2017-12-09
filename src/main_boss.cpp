#include "main_boss.h"

MainBoss::MainBoss(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, double velocity_x,
    double velocity_y, int health, Map* map_p)
    : Boss_Enemy(renderer, filename, error_handler, width, height, pos_x,
    pos_y, velocity_x, velocity_y, health) {
    //tempVX = velocity_x;
    //tempVY = velocity_y;
    map = map_p;
}

void MainBoss::update(double seconds){

    if (dead){
        if(!clone){
            clone1->killed();
            clone2->killed();
        }
        return;
    } else if(phase == 0){
        collidable = false;
    } else {collidable = true;}

    if(clone){invincible = true;}

    if(frozen){
        velocityX = 0;
        velocityY = 0;
        Sprite::update(seconds);
        return;
    }
    if(phase >= 3){
        speedMultiplier = 1;
        if(clone){
            collidable = false;
            setPosition(0, 0);
            
        } else {
            Sprite::animate(seconds, MAIN_BOSS_ANGRY, MAIN_BOSS_ANGRY
              + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
            secondPhase(seconds);
        }
      
    } else if(phase == 2){ 
        velocityX = 0;
        velocityY = 0;
        if(!clone){
            firstPhase(seconds); 
            Sprite::animate(seconds, MAIN_BOSS_IDLE, MAIN_BOSS_IDLE
              + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
        }
    } else if(phase == 1){
        if(clone){
            SDL_SetTextureAlphaMod(getTexture(), 0);
        } else {
            setPosition(map->width/2 ,map->height/2 - 200);
        }
        if(target != nullptr){
            if(fireballs[0] != nullptr && !fireballs[0]->wasThrown() && fireballs[0]->doneWaiting()){
                targetX = target->getDoubleRect().x;
                targetY = target->getDoubleRect().y;
            }
            if(fireballs[0]->doneWaiting() || !fireballs[2]->doneWaiting())
                fireballs[0]->goTo(targetX, targetY);
            if(fireballs[1] != nullptr && !fireballs[1]->wasThrown() && fireballs[1]->doneWaiting()){
                targetX2 = target->getDoubleRect().x;
                targetY2 = target->getDoubleRect().y;
            }
            if(fireballs[1]->doneWaiting() && !fireballs[0]->doneWaiting()){
                fireballs[1]->goTo(targetX2, targetY2);
            }
            if(fireballs[2] != nullptr && !fireballs[2]->wasThrown() && fireballs[2]->doneWaiting()){
                targetX3 = target->getDoubleRect().x;
                targetY3 = target->getDoubleRect().y;
            }
            if(fireballs[2]->doneWaiting() && !fireballs[1]->doneWaiting()){
                fireballs[2]->goTo(targetX3, targetY3);
            }
                Sprite::animate(seconds, MAIN_BOSS_THROWR, MAIN_BOSS_THROWR
                  + 7, ENEMY_FPS*speedMultiplier);
                //fireballs[0]->goTo(map->width/2 ,map->height/2);
            
        }
    }
    Boss_Enemy::update(seconds);
    Sprite::update(seconds);


}
void MainBoss::notifyCollision(Image* img, doubleRect* intersection, bool){
    if(!img->isCharacter()){ return;}
    chase = false;
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
            setPosition(map->width/2 ,map->height/2 - 200);
            clone1->setPosition(map->width/2 + 275,map->height/2 - 200);
            clone2->setPosition(map->width/2 - 275,map->height/2 - 200);
            positionChosen = true;
            break;
        case 1:
            clone1->setPosition(map->width/2 ,map->height/2 - 200);
            setPosition(map->width/2 + 275,map->height/2 - 200);
            clone2->setPosition(map->width/2 - 275,map->height/2 - 200);
            positionChosen = true;
            break;
        case 0:
            clone2->setPosition(map->width/2 ,map->height/2 - 200);
            clone1->setPosition(map->width/2 + 275,map->height/2 - 200);
            setPosition(map->width/2 - 275,map->height/2 - 200);
            positionChosen = true;
            break;
        default:
            //random numbers, shouldn't get here
            setPosition(800,1074);
            clone1->setPosition(800, 1219);
            clone2->setPosition(800, 974);
            positionChosen = true;
            break;
    }
}
void MainBoss::firstPhase(double seconds){
    if(!positionChosen){choosePositions();}
    if(wasAttacked && !chase){
        invincible = true;
        SDL_SetTextureAlphaMod(getTexture(), 100);
        SDL_SetTextureAlphaMod(clone1->getTexture(), 100);
        SDL_SetTextureAlphaMod(clone2->getTexture(), 100);
        collidable = false;
        clone1->setCollidable(false);
        clone2->setCollidable(false);
        invincibilityTimer += seconds;
        if(invincibilityTimer > INVINCIBILITY_TIME){
            SDL_SetTextureAlphaMod(getTexture(), 255);
            SDL_SetTextureAlphaMod(clone1->getTexture(), 255);
            SDL_SetTextureAlphaMod(clone2->getTexture(), 255);
            collidable = true;
            clone1->setCollidable(true);
            clone1->setCollidable(true);
            positionChosen = false;
            wasAttacked = false;
            clone1->nullAttacked();
            clone2->nullAttacked();
            invincibilityTimer = 0;
            invincible = false;
            positionChosen = false;
        }
    } else if(clone1->attacked() || clone2->attacked()){
         SDL_SetTextureAlphaMod(clone1->getTexture(), 0);
         SDL_SetTextureAlphaMod(clone2->getTexture(), 0);
         clone1->setPosition(500, 500);
         clone2->setPosition(500, 500);
         followSprite();
         speedMultiplier = 3.5;
         invincible = true;
         chase = true;

        if(flipXVelocity || flipYVelocity){
            positionChosen = false;
            SDL_SetTextureAlphaMod(clone1->getTexture(), 255);
            SDL_SetTextureAlphaMod(clone2->getTexture(), 255);
            clone1->nullAttacked();
            clone2->nullAttacked();
            flipXVelocity = false;
            flipYVelocity = false;
            invincible = false;
            wasAttacked = false;
            chase = false;
        }
            
    }

}
void MainBoss::secondPhase(double seconds){
    if (shouldFollow != nullptr){
        if(!chase){ 
            setPosition(map->width/2 ,map->height/2 - 200);
            chase = true;
        }
        invincibilityTimer += seconds;
        if(invincibilityTimer < INVINCIBILITY_TIME){
            invincible = true;
            SDL_SetTextureColorMod(getTexture(), 255, 0, 0);
            attemptFollow();
        } else if(invincibilityTimer > INVINCIBILITY_TIME * 1.5){
            invincibilityTimer = 0;
        } else {
            invincible = false;
            SDL_SetTextureColorMod(getTexture(), 0, 255, 0);
            velocityX = 0;
            velocityY = 0;
        }

    }
    velocityY *= speedMultiplier;
    velocityX *= speedMultiplier;
    Sprite::update(seconds);
}
