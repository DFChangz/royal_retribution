#include "loading_state.h"

LoadingState::LoadingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

void LoadingState::setup(){
  images["0BG"] = new Sprite(engine->renderer, BG_FILENAME, errorHandler,
    0, 0, false);
  images["1start"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 140, "STARTING  IN", ROYAL_GOLD);
  for (int i = 0; i < countdown; i++) {
    s = "2count_" + std::to_string(i);
    images[s] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 200, std::to_string(i), ROYAL_GOLD);
  }
}

void LoadingState::load() {
  State::load();
  for (it = images.find("1start"); it != images.end(); it++) {
    if (it->first[0] == '2')
    {
      SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
    }
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
  }
}

void LoadingState::update(double seconds){
  State::update(seconds);
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    engine->quit();}, SDLK_ESCAPE); 
}

void LoadingState::advance(){
  s = "2count_" + std::to_string(countdown);
  if (countdown < 6) SDL_SetTextureAlphaMod(images[s]->getTexture(), 0);
  else SDL_SetTextureAlphaMod(images["1start"]->getTexture(), 0);
  countdown--;
  s = "2count_" + std::to_string(countdown);
  SDL_SetTextureAlphaMod(images[s]->getTexture(), 255);
}

LoadingState::~LoadingState(){}
