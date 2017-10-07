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
  images.push_back(new Sprite(engine->renderer, BG_FILENAME, errorHandler,
    0, 0, false));
  // ship img 1
  images.push_back(new Sprite(engine->renderer, SHIP_FILENAME, errorHandler,
    -180, 180, false));
  // earth img 2
  images.push_back(new Sprite(engine->renderer, PLANET_FILENAME, errorHandler,
    0, 0, false));
  // logo text 3
  images.push_back(new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 100, logo));
  // skip text 4
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, skip));
}

/* loads images */
void TitleState::load() {
  State::load();
  // make all textures transparent
  for (unsigned int i = 1; i < images.size(); i++) {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
  }
  // center some of the textures
  for (unsigned int i = 2; i < images.size() - 1; i++) {
    auto center = getCenterForImage(images[i]);
    images[i]->setPosition(std::get<0>(center), std::get<1>(center));
  }
  // position skip text
  auto center = getCenterForImage(images[4]);
  images[4]->setPosition(std::get<0>(center), std::get<1>(center) + 300);
}

/* updates the screen */
void TitleState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;
  if(!audioHandler.isPlaying()){audioHandler.play("intro");}
  // wrap space and fade in skip
  if (images[0]->getDestRect()->x <= WIDTH - images[0]->getDestRect()->w) {
    x = 0;
    y = images[0]->pos_y;

    images[0]->setPosition(x, y);
  }
  a0 = fadeIn(4, a0, seconds, 2.5);
  images[0]->velocityX = -1 * speed;
  // after 3.5 sec, fade in ship
  if (totalTime > 3.5 && totalTime < 7.5) {
    a1 = fadeIn(1, a1, seconds, 2.5);
    images[1]->velocityX = 4 * speed;
  }
  // after 7.5 sec, fade in logo
  if (totalTime > 7.5 && totalTime < 11.5) {
    a3 = fadeIn(3, a3, seconds, 2.5);
  }
  // after 11.5 sec, fade in earth
  if (totalTime > 11.5) {
    a2 = fadeIn(2, a2, seconds, 2.5);
    x = images[2]->pos_x - 2.5 * speed * seconds;
    y = images[2]->pos_y - 2.5 * speed * seconds;
    images[2]->setPosition(x, y);
    images[2]->getDestRect()->w += 5.0 * speed * seconds;
    images[2]->getDestRect()->h += 5.0 * speed * seconds;
  }
  // after 15.5 sec, transfer to menu
  if (totalTime > 15.5) {
    engine->setState("menu");
  }
}

/* fades in a texture */
int TitleState::fadeIn(int i, int a, double seconds, double mult) {
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
std::tuple<int, int> TitleState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

TitleState::~TitleState() {}
