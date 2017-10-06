/*
 * lose_state.cpp
 * creates game over screen
 */

#include "lose_state.h"

LoseState::LoseState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

/* setup images */
void LoseState::setup() {
  // game over text
  images.push_back(new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 200, logo, RED));
}

/* loads images */
void LoseState::load() {
  State::load();
  // make the text transparent
  SDL_SetTextureAlphaMod(images[0]->getTexture(), 0);
  // center the text
  auto center = getCenterForImage(images[0]);
  images[0]->setPosition(std::get<0>(center), std::get<1>(center));
}

/* updates the screen */
void LoseState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;

  // wrap and fade in scroll
  a0 = fadeIn(0, a0, seconds, 2.5);
  // after 15.5 sec, transfer to menu
  if (totalTime > 3) {
    totalTime = 0;
    Mix_HaltMusic();
    engine->setState("Highscore");
  }
}

/* fades in a texture */
int LoseState::fadeIn(int i, int a, double seconds, double mult) {
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
std::tuple<int, int> LoseState::getCenterForImage(Image* image) {
  x = WIDTH / 2 - image->getDestRect()->w / 2;
  y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

LoseState::~LoseState() {}
