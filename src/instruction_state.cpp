/*
 * instruction_state.cpp
 * creates intstruction scene
 */

#include "instruction_state.h"

InstructionState::InstructionState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_1, TILES_TXT,
    &collisionDetector);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_1_ADD);
  setup();
  load();
}

/* setup images */
void InstructionState::setup() {
  // king 0
  images["0king"] = new Character(engine->renderer, E_C_FILENAME, errorHandler,
    16, 25, 670, 540, &eventHandler, &audioHandler, this);
  camera.setCharacter(static_cast<Character*>(images["0king"]));
  // instructions
  images["1skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, skip, ROYAL_GOLD); 
  images["1i1"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i1, ROYAL_GOLD);
  images["1up"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, w);
  images["1left"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, a);
  images["1down"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, s);
  images["1right"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, d);
  images["1i2"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i2, ROYAL_GOLD);
  images["1shift"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, sh);
  images["1space"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, sp);
  images["1end"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, end, ROYAL_GOLD);
}

/* loads images */
void InstructionState::load() {
  State::load();
  // make all textures txt (!skip) transparent
  for (it = images.begin(); it != images.end(); it++) {
    if (it->first[0] == '1') {
      if (it->first != "1skip") {
        SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
      }
      auto center = getCenterForImage(it->second);
      it->second->setPosition(std::get<0>(center), std::get<1>(center));
      if (it->first == "1skip") {
        it->second->pos_y = HEIGHT - 100;
      } else if (it->first == "1up" || it->first == "1shift") {
        it->second->pos_y -= 50;
      } else if (it->first == "1left") {
        it->second->pos_x -= 110;
      } else if (it->first == "1down" || it->first == "1space") {
        it->second->pos_y += 50;
      } else if (it->first == "1right") {
        it->second->pos_x += 114;
      } else {
        it->second->pos_y = 100;
      }
    }
  }
}

/* updates the screen */
void InstructionState::update(double seconds) {
  State::update(seconds);

  // skip intro
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    engine->setState("playing");}, SDLK_1);

  // fade in move text
  a0 = fadeIn("1i1", a0, seconds, 4);
  a1 = fadeIn("1up", a1, seconds, 4);
  a2 = fadeIn("1left", a2, seconds, 4);
  a3 = fadeIn("1down", a3, seconds, 4);
  a4 = fadeIn("1right", a4, seconds, 4);

  // determine of player pressed move buttons
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1up"]->getTexture(), 0, 255, 0);
    up = true;
  }, SDLK_w);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1up"]->getTexture(), 0, 255, 0);
    up = true;
  }, SDLK_UP);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1left"]->getTexture(), 0, 255, 0);
    left = true;
  }, SDLK_a);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1left"]->getTexture(), 0, 255, 0);
    left = true;
  }, SDLK_LEFT);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1down"]->getTexture(), 0, 255, 0);
    down = true;
  }, SDLK_s);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1down"]->getTexture(), 0, 255, 0);
    down = true;
  }, SDLK_DOWN);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1right"]->getTexture(), 0, 255, 0);
    right = true;
  }, SDLK_d);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1right"]->getTexture(), 0, 255, 0);
    right = true;
  }, SDLK_RIGHT);

  // if all move buttons pressed
  if (up && down && left && right) {
    SDL_SetTextureAlphaMod(images["1i1"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images["1up"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images["1left"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images["1down"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images["1right"]->getTexture(), 0);
    a5 = fadeIn("1i2", a5, seconds, 4);
    a6 = fadeIn("1shift", a6, seconds, 4);
    a7 = fadeIn("1space", a7, seconds, 4);
  }

  // determine if all action buttons were pressed
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1shift"]->getTexture(), 0, 255, 0);
    run = true;
  }, SDLK_LSHIFT);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["1space"]->getTexture(), 0, 255, 0);
    attack = true;
  }, SDLK_SPACE);

  // if so, end
  if (run && attack) {
    SDL_SetTextureAlphaMod(images["1i2"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images["1shift"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images["1space"]->getTexture(), 0);
    a8 = fadeIn("1end", a8, seconds, 4);
    eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
      engine->setState("playing");}, SDLK_n);
  }
}

/* center positions */
std::tuple<int, int> InstructionState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

InstructionState::~InstructionState() {}
