/*
 * credit_state.cpp
 */

#include "credit_state.h"

CreditState::CreditState(Engine* eng, ErrorHandler* eHandler)
  :  State(eng, eHandler) {
     setup();

    load();
}
void CreditState::setup(){

  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    HEIGHT, 40, "Credits" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[0]->getDestRect()->y + 60, 40, "Programmers" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[1]->getDestRect()->y + 60, 30, "Idean Labib" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[2]->getDestRect()->y + 60, 30, "Tae Jin Kim" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[3]->getDestRect()->y + 60, 30, "Billy Carrington" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[4]->getDestRect()->y + 60, 40, " Graphics" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[5]->getDestRect()->y + 60, 30, "Derek Chang" ));
}

void CreditState::load() {
  State::load();
  int x;
  int y;
  for(unsigned i = 0; i < images.size(); i++) {
    x = WIDTH / 2 - images[i]->getDestRect()->w / 2;
    y = images[i]->getDestRect()->y;
    images[i]->setPosition(x, y);
  }

  for(Image* words: images) {
    words->velocityY = -80;
  }
}

void CreditState::update(double seconds){
  State::update(seconds);

  if(images[0]->getDestRect()->y <= 0) {
    for(Image* words: images){
      words->velocityY = 0;
    }
    SDL_SetTextureColorMod(images[1]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images[5]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images[2]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images[3]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images[4]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images[6]->getTexture(), 0, 80, 190);
  }

} 

CreditState::~CreditState(){}
