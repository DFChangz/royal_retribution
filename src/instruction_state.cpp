/*
 * instruction_state.cpp
 * creates intstruction scene
 */

#include "instruction_state.h"

InstructionState::InstructionState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_0, TILES_TXT,
    &collisionDetector);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_0_ADD);

  setup();
  load();
}

/* setup images */
void InstructionState::setup() {
  // king 0
  images["king"] = new Character(engine->renderer, E_C_FILENAME, errorHandler,
    16, 25, 855, 540, &eventHandler, &audioHandler, this);
  camera.setCharacter(static_cast<Character*>(images["king"]));
  // instructions
  images["skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, skip, ROYAL_GOLD); 
  images["i1"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i1, ROYAL_GOLD);
  images["up"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, w);
  images["left"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, a);
  images["down"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, s);
  images["right"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, d);
  images["i2"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i2, ROYAL_GOLD);
  images["shift"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, sh);
  images["space"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, sp);
  images["end"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, end, ROYAL_GOLD);
}

/* loads images */
void InstructionState::load() {
  State::load();
  // make all textures txt (!skip) transparent
  for (it = images.find("i1"); it != images.end(); it++) {
    SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
  }
  // position all the text
  for (it = images.find("skip"); it != images.end(); it++) {
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
    if (it->first == "skip") {
      it->second->pos_y = HEIGHT - 100;
    } else if (it->first == "up" || it->first == "shift") {
      it->second->pos_y -= 50;
    } else if (it->first == "left") {
      it->second->pos_x -= 110;
    } else if (it->first == "down" || it->first == "space") {
      it->second->pos_y += 50;
    } else if (it->first == "right") {
      it->second->pos_x += 114;
    } else {
      it->second->pos_y = 100;
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
  a0 = fadeIn("i1", a0, seconds, 4);
  a1 = fadeIn("up", a1, seconds, 4);
  a2 = fadeIn("left", a2, seconds, 4);
  a3 = fadeIn("down", a3, seconds, 4);
  a4 = fadeIn("right", a4, seconds, 4);

  // determine of player pressed move buttons
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["up"]->getTexture(), 0, 255, 0);
    up = true;
  }, SDLK_w);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["up"]->getTexture(), 0, 255, 0);
    up = true;
  }, SDLK_UP);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["left"]->getTexture(), 0, 255, 0);
    left = true;
  }, SDLK_a);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["left"]->getTexture(), 0, 255, 0);
    left = true;
  }, SDLK_LEFT);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["down"]->getTexture(), 0, 255, 0);
    down = true;
  }, SDLK_s);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["down"]->getTexture(), 0, 255, 0);
    down = true;
  }, SDLK_DOWN);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["right"]->getTexture(), 0, 255, 0);
    right = true;
  }, SDLK_d);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["right"]->getTexture(), 0, 255, 0);
    right = true;
  }, SDLK_RIGHT);

  // if all move buttons pressed
  if (up && down && left && right) {
    for (it = images.find("i1"); it != images.find("i2"); it++) {
      SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
    }
    a5 = fadeIn("i2", a5, seconds, 4);
    a6 = fadeIn("shift", a6, seconds, 4);
    a7 = fadeIn("space", a7, seconds, 4);
  }

  // determine if all action buttons were pressed
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["shift"]->getTexture(), 0, 255, 0);
    run = true;
  }, SDLK_LSHIFT);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images["space"]->getTexture(), 0, 255, 0);
    attack = true;
  }, SDLK_SPACE);

  // if so, end
  if (run && attack) {
    for (it = images.find("i2"); it != images.find("end"); it++) {
      SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
    }
    a8 = fadeIn("end", a8, seconds, 4);
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
