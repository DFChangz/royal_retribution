#include "loading_state.h"

LoadingState::LoadingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

void LoadingState::setup(){

  images["0BG"] = new Sprite(engine->renderer, BG_FILENAME, errorHandler,
    0, 0, false);
  images["1Load"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 120, "L O A D I N G", ROYAL_GOLD);
}
void LoadingState::load() {
  State::load();
  auto center = getCenterForImage(images["1Load"]);
  images["1Load"]->setPosition(std::get<0>(center), std::get<1>(center));
}

void LoadingState::update(double seconds){
  State::update(seconds);  
}
void LoadingState::loadingStart(){
  SDL_SetTextureAlphaMod(images["1Load"]->getTexture(), 255);
  SDL_SetTextureAlphaMod(images["0BG"]->getTexture(), 255);
}
void LoadingState::loadingEnd(){
  auto center = getCenterForImage(images["1Load"]);
  images["1Load"]->setPosition(std::get<0>(center), std::get<1>(center));
  SDL_SetTextureAlphaMod(images["1Load"]->getTexture(), 255);
  SDL_SetTextureColorMod(images["1Load"]->getTexture(), 255, 189, 27);
}
void LoadingState::loadingMove(double x, double y){
  images["1Load"]->setPosition(images["1Load"]->pos_x + x, images["1Load"]->pos_y + y);
}
void LoadingState::changeColor(int r, int g, int b){
  SDL_SetTextureColorMod(images["1Load"]->getTexture(), r, g, b);

}
LoadingState::~LoadingState(){}
