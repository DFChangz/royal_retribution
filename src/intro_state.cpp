/*
 * intro_state.cpp
 * creates intro scene
 */

#include "intro_state.h"

IntroState::IntroState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_1, TILES_TXT,
    &collisionDetector);

  setup();
  load();
}

/* setup images */
void IntroState::setup() {
  // throne
  images["0throne"] = new Sprite(engine->renderer, THRONE_FILENAME,
     errorHandler, 1854, 2160, false);
  // king 
  images["1king"] = new Character(engine->renderer, ANI_FILENAME, errorHandler,
    16, 25, 1902, 2209, &eventHandler, &audioHandler, this);
  // squire
  images["1squire"] = new Squire(engine->renderer, SQUIRE_FILENAME,
    errorHandler, 16, 25, 1902, 2677, 0, 0);
  camera.setCharacter(static_cast<Character*>(images["1squire"]));
  // text
  images["2c0"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s1);
  images["2c1"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k1);
  images["2c2"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s2);
  images["2c3"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k2);
  images["2c4"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s3);
  images["2c5"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k3);
  images["2c6"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k4);
  images["2c7"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 40, s4, ROYAL_GOLD);
  images["2skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, skip);
}

/* loads images */
void IntroState::load() {
  State::load();
  // set up throne
  images["0throne"]->getDestRect()->w *= 2;
  images["0throne"]->getDestRect()->h *= 2;
  // squre faces left and king is frozen
  static_cast<Squire*>(images["1squire"])->dir = "left";
  static_cast<Character*>(images["1king"])->frozen = true;
  // make convo textures transparent
  for (it = images.find("2c0"); it != images.end(); it++) {
    if (it->first != "2skip")
    SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
  }
  // position all the text
  for (it = images.find("2c0"); it != images.end(); it++) {
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
    if (it->first == "2skip") {
      it->second->pos_y = 0;
    } else if (it->first == "2c7") {
      it->second->pos_y += 280;
    } else if (it->first == "2c0" || it->first == "2c2" || it->first == "2c4") {
      it->second->pos_y -= 50;
    } else {
      it->second->pos_y -= 285;
    }
  }
}

/* updates the screen */
void IntroState::update(double seconds) {
  State::update(seconds);
  // skip intro
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    engine->setState("instruction");}, SDLK_1);

  std::string s = "2c" + std::to_string(counter);
  std::string prev = "2c" + std::to_string(counter-1);

  switch(event) {
    // squire walks in
    case 0:
      if (images["1squire"]->pos_x > 1265) {
        images["1squire"]->velocityX = -300;
      } else {
        images["1squire"]->velocityX = 0;
        event++;
      }
      break;
    case 1:
      if (images["1squire"]->pos_y > 2405) {
        images["1squire"]->velocityY = -300;
      } else {
        images["1squire"]->velocityY = 0;
        event++;
      }
      break;
    case 2:
      if (images["1squire"]->pos_x < 1902) {
        images["1squire"]->velocityX = 300;
      } else {
        event++;
        images["1squire"]->velocityX = 0;
        static_cast<Squire*>(images["1squire"])->dir = "up";
      }
      break;
  }
  // text displayed with [n]
  if (event == 3) {
    if (counter < 7) {
      SDL_SetTextureAlphaMod(images[s]->getTexture(), 255);
      if (counter > 0) {
        SDL_SetTextureAlphaMod(images[prev]->getTexture(), 0);
      }
      if (counter >= 5) {
        if (images["1king"]->pos_y < 2287) {
          images["1king"]->velocityY = 100;
        } else  images["1king"]->velocityY = 0;
      }
      // increments to the next text
      eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
        raise = true;}, SDLK_n);
      if (raise) { raise = false; counter++; }
    } else {
      SDL_SetTextureAlphaMod(images[prev]->getTexture(), 0);
      // edit king movement
      images["1king"]->velocityX = 150;
      if (images["1king"]->pos_y > 2287) images["1king"]->velocityY = 0;
      // edit squire movement
      if (images["1king"]->pos_x > 1910 && !end) {
        static_cast<Squire*>(images["1squire"])->dir = "right";
        end = true;
      }
      if (images["1king"]->pos_x > 2550) {
        a = fadeIn("2c7", a, seconds, 2);
        static_cast<Squire*>(images["1squire"])->dir = "down";
        eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
        static_cast<Character*>(images["1king"])->frozen = false;
          engine->setState("instruction");}, SDLK_n);
      }
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
