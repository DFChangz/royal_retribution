/*
 * win_state.cpp
 * creates you won screen
 */

#include "win_state.h"

WinState::WinState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

/* setup images */
void WinState::setup() {
  // game over text
  images.push_back(new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 200, logo, ROYAL_GOLD));
}

/* loads images */
void WinState::load() {
  State::load();
  // make the text transparent
  SDL_SetTextureAlphaMod(images[0]->getTexture(), 0);
  // center the text
  auto center = getCenterForImage(images[0]);
  images[0]->setPosition(std::get<0>(center), std::get<1>(center));
  /* input score into highscore
  std::ofstream file;
  file.open(SCORE_FILENAME, std::ios_base::app);
  file << std::to_string(engine->score) << std::endl;
  file.close(); */
}

/* updates the screen */
void WinState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;

  // wrap and fade in scroll
  a0 = fadeIn(0, a0, seconds, 2.5);
  // after 15.5 sec, transfer to menu
  if (totalTime > 3) {
  Mix_HaltMusic();
    engine->setState("menu");
  }
}

/* fades in a texture */
int WinState::fadeIn(int i, int a, double seconds, double mult) {
  newA = (double)a + speed * seconds * mult;
  if (newA < 255) {
    a = (int)newA;
    SDL_SetTextureAlphaMod(images[i]->getTexture(), a);
  } else {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 255);
  }
  return a;
}

/* center positions */
std::tuple<int, int> WinState::getCenterForImage(Image* image) {
  x = WIDTH / 2 - image->getDestRect()->w / 2;
  y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

WinState::~WinState() {}
