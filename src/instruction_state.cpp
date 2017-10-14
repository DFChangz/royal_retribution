/*
 * instruction_state.cpp
 * creates intstruction scene
 */

#include "instruction_state.h"

InstructionState::InstructionState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_0, TILES_TXT);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_0_ADD);

  setup();
  load();
}

/* setup images */
void InstructionState::setup() {
  // king 0
  images.push_back(new Character(engine->renderer, E_C_FILENAME, errorHandler,
    16, 25, 855, 540, &eventHandler, &audioHandler, this));
  camera.setCharacter(static_cast<Character*>(images[0]));
  // move instruct
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, skip, RED)); 
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i1, ROYAL_GOLD));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, w));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, a));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, s));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, d));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i2, ROYAL_GOLD));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, sh));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, sp));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, end, ROYAL_GOLD));
}

/* loads images */
void InstructionState::load() {
  State::load();
  // make all textures (!king&!squire) transparent
  for (unsigned int i = 2; i < images.size(); i++) {
    SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
  }
  // position all the text
  for (unsigned int i = 1; i < images.size(); i++) {
    auto center = getCenterForImage(images[i]);
    images[i]->setPosition(std::get<0>(center), std::get<1>(center));
    if (i == 1) {
      images[i]->pos_y = HEIGHT - 100;
    } else if (i == 3 || i == 8) {
      images[i]->pos_y -= 50;
    } else if (i == 4) {
      images[i]->pos_x -= 110;
    } else if (i == 5 || i == 9) {
      images[i]->pos_y += 50;
    } else if (i == 6) {
      images[i]->pos_x += 114;
    } else {
      images[i]->pos_y = 100;
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
  a2 = fadeIn(2, a2, seconds, 4);
  a3 = fadeIn(3, a3, seconds, 4);
  a4 = fadeIn(4, a4, seconds, 4);
  a5 = fadeIn(5, a5, seconds, 4);
  a6 = fadeIn(6, a6, seconds, 4);

  // determine of player pressed move buttons
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    up = true; SDL_SetTextureColorMod(images[3]->getTexture(), 0, 255, 0);
  }, SDLK_w);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    up = true; SDL_SetTextureColorMod(images[3]->getTexture(), 0, 255, 0);
  }, SDLK_UP);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    left = true; SDL_SetTextureColorMod(images[4]->getTexture(), 0, 255, 0);
  }, SDLK_a);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    left = true; SDL_SetTextureColorMod(images[4]->getTexture(), 0, 255, 0);
  }, SDLK_LEFT);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    down = true; SDL_SetTextureColorMod(images[5]->getTexture(), 0, 255, 0);
  }, SDLK_s);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    down = true; SDL_SetTextureColorMod(images[5]->getTexture(), 0, 255, 0);
  }, SDLK_DOWN);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    right = true; SDL_SetTextureColorMod(images[6]->getTexture(), 0, 255, 0);
  }, SDLK_d);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    right = true; SDL_SetTextureColorMod(images[6]->getTexture(), 0, 255, 0);
  }, SDLK_RIGHT);

  // if all move buttons pressed
  if (up && down && left && right) {
    for (int i = 2; i < 7; i++) {
      SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
    }
    a7 = fadeIn(7, a7, seconds, 4);
    a8 = fadeIn(8, a8, seconds, 4);
    a9 = fadeIn(9, a9, seconds, 4);
  }

  // determine if all action buttons were pressed
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    run = true; SDL_SetTextureColorMod(images[8]->getTexture(), 0, 255, 0);
  }, SDLK_LSHIFT);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    attack = true; SDL_SetTextureColorMod(images[9]->getTexture(), 0, 255, 0);
  }, SDLK_SPACE);

  // if so, end
  if (run && attack) {
    for (int i = 7; i < 10; i++) {
      SDL_SetTextureAlphaMod(images[i]->getTexture(), 0);
    }
    a10 = fadeIn(10, a10, seconds, 3);
    eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
      engine->setState("playing");}, SDLK_n);
  }
}

/* fades in a texture */
int InstructionState::fadeIn(int i, int a, double seconds, double mult) {
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
std::tuple<int, int> InstructionState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

InstructionState::~InstructionState() {}
