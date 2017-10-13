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
  std::cout << "done\n";
}

/* setup images */
void IntroState::setup() {
  // king 0
  images.push_back(new Character(engine->renderer, E_C_FILENAME, errorHandler,
    16, 25, /*2546*/80, /*1125*/80, &eventHandler, &audioHandler, this));
  camera.setCharacter(static_cast<Character*>(images[0]));
  // squire 1
  images.push_back(new Character(engine->renderer, E_C_FILENAME, errorHandler,
    16, 25, /*2546*/80, /*1395*/80, &eventHandler, &audioHandler, this));
  // text 2-9
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    500, 300, 30, s1));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    500, 100, 30, k1));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    500, 300, 30, s2));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    500, 100, 30, k2));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    500, 300, 30, s3));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    500, 100, 30, k3));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    500, 100, 30, k4));
  images.push_back(new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    500, 300, 30, s4));
}

/* loads images */
void IntroState::load() {
  State::load();
  // make all textures (except king) transparent
  for (unsigned int i = 1; i < images.size(); i++) {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
  }
}

/* updates the screen */
void IntroState::update(double seconds) {
  State::update(seconds);
  // inc totalTime
  totalTime += seconds;

  // fade in squire
  if (totalTime < 2) {
    static_cast<Character*>(images[1])->dir = "up";
    fadeIn(1, a1, seconds, speed);
  // squire walks up
  } else {
    images[1]->velocityY = (totalTime < 3.5) ? -50 : 0;
  }
  // text displayed with [space]
  if (totalTime > 3.5) {
    if (counter < 9) {
      SDL_SetTextureAlphaMod(images[counter]->getTexture(), 255);
      eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
        counter++; }, SDLK_SPACE);
      SDL_SetTextureAlphaMod(images[counter-1]->getTexture(), 0);
    } else {
      specificTime = totalTime;
    }
  }
  if (counter == 9) {
    SDL_SetTextureAlphaMod(images[counter-1]->getTexture(), 0);
    images[0]->velocityY = -100;
    if (totalTime > specificTime + 1) {
      if (totalTime < specificTime + 1.5) {
        camera.setCharacter(static_cast<Character*>(images[0]));
        images[1]->velocityX = -50;
      } else {
        images[1]->velocityX = 0;
        static_cast<Character*>(images[1])->dir = "down";
        SDL_SetTextureAlphaMod(images[counter]->getTexture(), 255);
        eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
          engine->setState("playing"); }, SDLK_SPACE);
      }
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

IntroState::~IntroState() {}
