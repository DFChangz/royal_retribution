/*
 * credit_state.cpp
 */

#include "credit_state.h"

CreditState::CreditState(Engine* eng, ErrorHandler* eHandler)
  :  State(eng, eHandler) {

    setup();
    std::cout << "finish setup\n";
    load();
    std::cout << "finish load\n";
}
void CreditState::setup(){
  images["t0"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, HEIGHT, 40, "Credits" );
  images["t1"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t0"]->getDestRect()->y + 60, 40, "Programmers");
  images["t2"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t1"]->getDestRect()->y + 60, 30, "Idean Labib");
  images["t3"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t2"]->getDestRect()->y + 60, 30, "Tae Jin Kim");
  images["t4"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t3"]->getDestRect()->y + 60, 30, "Billy Carrington");
  images["t5"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t4"]->getDestRect()->y + 60, 40, " Graphics");
  images["t6"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t5"]->getDestRect()->y + 60, 30, "Derek Chang");
}

void CreditState::load() {
  State::load();
  int x;
  int y;
  for (it = images.begin(); it != images.end(); it++) {
    x = WIDTH / 2 - it->second->getDestRect()->w / 2;
    y = it->second->getDestRect()->y;
    it->second->setPosition(x, y);
    it->second->velocityY = -200;
  }
}

void CreditState::update(double seconds){
  State::update(seconds);

  if(images["t0"]->getDestRect()->y <= 0) {
    for(it = images.begin(); it != images.end(); it++){
      it->second->velocityY = 0;
    }
    SDL_SetTextureColorMod(images["t1"]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images["t2"]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images["t3"]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images["t4"]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images["t5"]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images["t6"]->getTexture(), 0, 80, 190);
  }
} 

CreditState::~CreditState(){}
