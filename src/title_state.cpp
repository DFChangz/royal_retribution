/*
 * title_state.cpp
 * creates title screen
 */

#include "title_state.h"

TitleState::TitleState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

void TitleState::setup() {
  // setup texture for font
  images.push_back(new Sprite(engine->renderer, BG_FILENAME, errorHandler,
    0, 0, false));
  images.push_back(new Sprite(engine->renderer, SHIP_FILENAME, errorHandler,
    -200, 200, false));
  images.push_back(new Sprite(engine->renderer, PLANET_FILENAME, errorHandler,
    0, 0, false));
  images.push_back(new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 100, logo));
}

void TitleState::load() {
  State::load();

  for (unsigned int i = 0; i < images.size(); i++) {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
  }

  // change ship size
  images[1]->getDestRect()->w = 400;
  images[1]->getDestRect()->h = 300;

  for (unsigned int i = 2; i < images.size(); i++) {
    auto center = getCenterForImage(images[i]);
    images[i]->setPosition(std::get<0>(center), std::get<1>(center));
  }

  images[0]->velocityX = scroll_speed;
}

/* updates the screen */
void TitleState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;

  // wrap and fade in scroll
  if (images[0]->getDestRect()->x <= WIDTH - images[0]->getDestRect()->w) {
    x = 0;
    y = images[0]->pos_y;

    images[0]->setPosition(x, y);
  }
  fadeIn(0, seconds, 0.2);
  // after 3.5 sec, fade in ship
  if (totalTime > 3.5) {
    fadeIn(1, seconds, 0.2);
    images[1]->velocityX = -4 * scroll_speed;
  }
  // after 7.5 sec, fade in logo
  if (totalTime > 7.5) {
    fadeIn(3, seconds, 0.2);
  }
  // after 11.5 sec, fade in earth
  if (totalTime > 11.5) {
    fadeIn(2, seconds, 0.2);
    x = images[2]->pos_x - 2.5 * fade_speed * seconds;
    y = images[2]->pos_y - 2.5 * fade_speed * seconds;
    images[2]->setPosition(x, y);
    images[2]->getDestRect()->w += 5.0 * fade_speed * seconds;
    images[2]->getDestRect()->h += 5.0 * fade_speed * seconds;
  }
  // after 15.5 sec, transfer to menu
  if (totalTime > 15.5) {
    engine->setState("menu");
  }
}

/* fades in a texture */
void TitleState::fadeIn(int i, double seconds, double mult) {
  SDL_SetTextureAlphaMod(images[i]->getTexture(), alpha);
  if (alpha < 255) {
    newAlpha += fade_speed * seconds * mult;
    alpha = (int)newAlpha;
  }
  if (alpha > 255) {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 255);
    alpha = 0;
    newAlpha = 0;
  }
}

/* centers image */
std::tuple<int, int> TitleState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}


TitleState::~TitleState() {}
