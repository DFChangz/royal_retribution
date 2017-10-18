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

/* setup images */
void TitleState::setup() {
  // space BG 0
  images["BG"] = new Sprite(engine->renderer, BG_FILENAME, errorHandler,
    0, 0, false);
  // ship img 1
  images["ship"] = new Sprite(engine->renderer, SHIP_FILENAME, errorHandler,
    -180, 180, false);
  // earth img 2
  images["earth"] = new Sprite(engine->renderer, PLANET_FILENAME, errorHandler,
    0, 0, false);
  // logo text 3
  images["logo"] = new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 100, logo);
  // skip text 4
  images["skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, skip);
}

/* loads images */
void TitleState::load() {
  State::load();
  // make all textures transparent
  for (it = images.find("ship"); it != images.end(); it++) {
    SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
  }
  // center some of the textures
  for (it = images.find("earth"); it != images.find("logo"); it++) {
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
  }
  // position skip text
  auto center = getCenterForImage(images["ship"]);
  images["ship"]->setPosition(std::get<0>(center), std::get<1>(center) + 300);
}

/* updates the screen */
void TitleState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;
  if(!audioHandler.isPlaying()){audioHandler.play("intro");}
  // wrap space and fade in skip
  if (images["BG"]->getDestRect()->x <= WIDTH - images["BG"]->getDestRect()->w)
  {
    x = 0;
    y = images["BG"]->pos_y;
    images["BG"]->setPosition(x, y);
  }
  a0 = fadeIn("ship", a0, seconds, 2.5);
  images["BG"]->velocityX = -4 * speed;
  // after 3.5 sec, fade in ship
  if (totalTime > 3.5 && totalTime < 7.5) {
    a1 = fadeIn("ship", a1, seconds, 2.5);
    images["ship"]->velocityX = 4 * speed;
  }
  // after 7.5 sec, fade in logo
  if (totalTime > 7.5 && totalTime < 11.5) {
    a3 = fadeIn("logo", a3, seconds, 2.5);
  }
  // after 11.5 sec, fade in earth
  if (totalTime > 11.5) {
    a2 = fadeIn("earth", a2, seconds, 2.5);
    x = images["earth"]->pos_x - 2.5 * speed * seconds;
    y = images["earth"]->pos_y - 2.5 * speed * seconds;
    images["earth"]->setPosition(x, y);
    images["earth"]->getDestRect()->w += 5.0 * speed * seconds;
    images["earth"]->getDestRect()->h += 5.0 * speed * seconds;
  }
  // after 15.5 sec, transfer to menu
  if (totalTime > 15.5) {
    engine->setState("menu");
  }
}

/* center positions */
std::tuple<int, int> TitleState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

TitleState::~TitleState() {}
