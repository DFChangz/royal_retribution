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
    0, 0, false));
  images.push_back(new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 80, logo));
  images.push_back(new Sprite(engine->renderer, PLANET_FILENAME, errorHandler,
    0, 0, false));
}

void TitleState::load() {
  State::load();

  for (unsigned int i = 0; i < images.size(); i++) {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
  }

  images[1]->getDestRect()->w = 400;
  images[1]->getDestRect()->w = 300;

  for (unsigned int i = 0; i < images.size(); i++) {
    auto center = getCenterForImage(images[i]);
    images[i]->setPosition(std::get<0>(center), std::get<1>(center));
  }

  images[0]->velocityX = scroll_speed;
}

/* updates the screen */
void TitleState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;

  // wrapping the scroll img
  if (images[0]->getDestRect()->x <= WIDTH - images[0]->getDestRect()->w) {
    int x = 0;
    int y = images[0]->pos_y;

    images[0]->setPosition(x, y);
  }
  // fade in ship and scroll
  if (totalTime > 4) {
    fadeIn(1, seconds);
  }
  // after 7.5 sec, fade in logo
  if (totalTime > 7.5) {
    fadeIn(2, seconds);
  }
  // after 14.5 sec, flash to earth
  if (totalTime > 14.5) {
    for (unsigned int i = 0; i < images.size(); i++) {
      SDL_SetTextureColorMod(images[i]->getTexture(), 255, 255, 255);
    }
  }
  if (totalTime > 15) {
    fadeIn(3, seconds);
  }
  if (totalTime > 16) {
    engine->setState("menu");
  }
}

/* fades in a texture */
void TitleState::fadeIn(int i, double seconds) {
  SDL_SetTextureAlphaMod(images[i]->getTexture(), alpha);
  if (alpha < 255) {
    newAlpha += fade_speed * seconds;
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
