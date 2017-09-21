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
  images.push_back(new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 40, logo));
  images.push_back(new Sprite(engine->renderer, SHIP_FILENAME, errorHandler,
    0, 0, false));
  images.push_back(new Sprite(engine->renderer, BG_FILENAME, errorHandler,
    0, 0, false));
  images.push_back(new Sprite(engine->renderer, PLANET_FILENAME, errorHandler,
    0, 0, false));
}

void TitleState::load() {
  State::load();

  for (unsigned int i = 0; i < images.size(); i++) {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
  }
}

/* updates the screen */
void TitleState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;
  // wrapping the scroll img
  if (images[2]->getDestRect()->x <= WIDTH - images[2]->getDestRect()->w) {
    images[2]->getDestRect()->x = 0;
  } else {
    int x = images[2]->getDestRect()->x;
    int y = images[2]->getDestRect()->y;
    images[2]->setPosition(x - scroll_speed * seconds, y);
  }
  // fade in ship and scroll
  if (totalTime > 4) {
    fadeIn(1, seconds);
  }
  // after 7.5 sec, fade in logo
  if (totalTime > 7.5) {
    fadeIn(0, seconds);
  }
  // after 14.5 sec, flash to earth
  if (totalTime > 14.5) {
    for (int i = 0; i < 3; i++) {
      SDL_SetTextureColorMod(images[i]->getTexture(), 255, 255, 255);
    }
    fadeIn(3, seconds);
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

TitleState::~TitleState() {}
