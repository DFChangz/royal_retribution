/*
 * intro_state.cpp
 * creates intro scene
 */

#include "intro_state.h"

IntroState::IntroState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_0, TILES_TXT,
    &collisionDetector);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_0_ADD);

  setup();
  load();
}

/* setup images */
void IntroState::setup() {
  // throne
  images["throne"] = new Sprite(engine->renderer, THRONE_FILENAME, errorHandler,
    2530, 1370, false);
  // king 
  images["king"] = new Character(engine->renderer, E_C_FILENAME, errorHandler,
    16, 25, 2546, 1320, &eventHandler, &audioHandler, this);
  // squire
  images["squire"] = new Squire(engine->renderer, SQUIRE_FILENAME, errorHandler,
    16, 25, 2546, 1014, 0, 0);
  camera.setCharacter(static_cast<Character*>(images["squire"]));
  // text
  images["skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, skip);
  images["c0"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s1);
  images["c1"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k1);
  images["c2"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s2);
  images["c3"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k2);
  images["c4"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s3);
  images["c5"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k3);
  images["c6"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k4);
  images["c7"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 40, s4, ROYAL_GOLD);
}

/* loads images */
void IntroState::load() {
  State::load();
  // king starts facing up & frozen
  static_cast<Character*>(images["king"])->dir = "up";
  static_cast<Character*>(images["king"])->frozen = true;;
  // make convo textures transparent
  for (it = images.find("s1"); it != images.end(); it++) {
    SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
  }
  // position all the text
  for (it = images.find("skip"); it != images.end(); it++) {
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
    if (it->first == "skip") {
      it->second->pos_y = 0;
    } else if (it->first == "c7") {
      it->second->pos_y -= 114;
    } else if (it->first == "c0" || it->first == "c2" || it->first == "c4") {
      it->second->pos_y -= 54;
    } else {
      it->second->pos_y += 144;
    }
  }
}

/* updates the screen */
void IntroState::update(double seconds) {
  State::update(seconds);

  // skip intro
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    engine->setState("instruction");}, SDLK_1);

  std::string s = "c" + counter;
  std::string prev = "c" + counter-1;

  // squire walks in
  if (images["squire"]->pos_y < 1125) {
    images["squire"]->velocityY = 100;
  } else {
    images["squire"]->velocityY = 0;
    // text displayed with [n]
    if (counter < 7) {
      SDL_SetTextureAlphaMod(images[s]->getTexture(), 255);
      if (counter > 0) {
        SDL_SetTextureAlphaMod(images[prev]->getTexture(), 0);
      }
      // increments to the next text
      eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
        raise = true;}, SDLK_n);
      if (raise) { raise = false; counter++; }
      //eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
        //raise = false;}, SDLK_n);
    }
  }
  if (counter == 7) {
    SDL_SetTextureAlphaMod(images[prev]->getTexture(), 0);
    // edit king movement
    if (images["king"]->pos_y > 1014) {
      images["king"]->velocityY = -150;
    } else {
      images["king"]->velocityY = 0;
      images["king"]->velocityX = 150;
    }
    // edit squire movement
    if (images["squire"]->pos_x > 2470) {
      images["squire"]->velocityX = -50;
    } else {
      images["squire"]->velocityX = 0;
      static_cast<Squire*>(images["squire"])->dir = "right";
    }
    // final quote
    if (images["king"]->pos_x > 3000) {
      a = fadeIn("c7", a, seconds, 2);
      eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
        static_cast<Character*>(images["king"])->frozen = false;
        engine->setState("instruction");}, SDLK_n);
    }
  }
}

/* center positions */
std::tuple<int, int> IntroState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

IntroState::~IntroState() {}
