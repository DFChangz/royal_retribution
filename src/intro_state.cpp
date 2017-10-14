/*
 * intro_state.cpp
 * creates intro scene
 */

#include "intro_state.h"

IntroState::IntroState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_0, TILES_TXT);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_0_ADD);

  setup();
  load();
}

/* setup images */
void IntroState::setup() {
  // throne 0
  images.push_back(new Sprite(engine->renderer, THRONE_FILENAME, errorHandler,
    64, 64, 2530, 1310, false));
  // king 1 
  images.push_back(new Character(engine->renderer, E_C_FILENAME, errorHandler,
    16, 25, 2546, 1320, &eventHandler, &audioHandler, this));
  // squire 2
  images.push_back(new Squire(engine->renderer, SQUIRE_FILENAME, errorHandler,
    16, 25, 2546, 1014, 0, 0));
  camera.setCharacter(static_cast<Character*>(images[2]));
  // text 3-11
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, skip));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s1));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k1));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s2));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k2));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s3));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k3));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k4));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 40, s4, ROYAL_GOLD));
  // throne
  
}

/* loads images */
void IntroState::load() {
  State::load();
  // king starts facing up
  static_cast<Character*>(images[1])->dir = "up";
  // make text textures transparent
  for (unsigned int i = 4; i < images.size(); i++) {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
  }
  // position all the text
  for (unsigned int i = 0; i < images.size(); i++) {
    if (i == 1 || i == 2) continue;
    auto center = getCenterForImage(images[i]);
    images[i]->setPosition(std::get<0>(center), std::get<1>(center));
    if (i == 3) {
      images[i]->pos_y = 0;
    } else if (i == 0) {
      images[i]->pos_y = 0;
    } else if (i == 11) {
      images[i]->pos_y -= 114;
    } else if (i == 4 || i == 6 || i == 8) {
      images[i]->pos_y -= 54;
    } else {
      images[i]->pos_y += 144;
    }
  }
}

/* updates the screen */
void IntroState::update(double seconds) {
  State::update(seconds);

  // skip intro
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    engine->setState("instruction");}, SDLK_1);

  // squire walks in
  if (images[2]->pos_y < 1125) {
    images[2]->velocityY = 100;
  } else {
    images[2]->velocityY = 0;
    // text displayed with [n]
    if (counter < 11) {
      SDL_SetTextureAlphaMod(images[counter]->getTexture(), 255);
      if (counter > 4) {
        SDL_SetTextureAlphaMod(images[counter-1]->getTexture(), 0);
      }
      // increments to the next text
      eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
        raise = true;}, SDLK_n);
      if (raise) { raise = false; counter++; }
      eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
        raise = false;}, SDLK_n);
    }
  }
  if (counter == 11) {
    SDL_SetTextureAlphaMod(images[counter-1]->getTexture(), 0);
    // edit king movement
    if (images[1]->pos_y > 1014) {
      images[1]->velocityY = -150;
    } else {
      images[1]->velocityY = 0;
      images[1]->velocityX = 150;
    }
    // edit squire movement
    if (images[2]->pos_x > 2470) {
      images[2]->velocityX = -50;
    } else {
      images[2]->velocityX = 0;
      static_cast<Squire*>(images[2])->dir = "right";
    }
    // final quote
    if (images[1]->pos_x > 3000) {
      a = fadeIn(counter, a, seconds, 2);
      eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
        engine->setState("instruction");}, SDLK_n);
    }
  }
}

/* fades in a texture */
int IntroState::fadeIn(int i, int a, double seconds, double mult) {
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
std::tuple<int, int> IntroState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

IntroState::~IntroState() {}
