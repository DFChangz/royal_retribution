/*
 * lose_state.cpp
 * creates game over screen
 */

#include "lose_state.h"

LoseState::LoseState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

/* setup images */
void LoseState::setup() {
  // king
  images["king"] = new Character(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, 125, 118, &eventHandler, &audioHandler, this);
  static_cast<Character*>(images["king"])->frozen = true;
  // game over text
  images["lose"] = new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 200, logo, RED);
  
}

/* loads images */
void LoseState::load() {
  State::load();

  images["king"]->getDestRect()->w = 128;
  images["king"]->getDestRect()->h = 200;
  SDL_SetTextureAlphaMod(images["lose"]->getTexture(), 0);

  for (it = images.begin(); it != images.end(); it++) {
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
  }
}

/* updates the screen */
void LoseState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;

  // death animation
  if (static_cast<Character*>(images["king"])->isDead()) {
    a0 = fadeIn("lose", a0, seconds, 3);
    a1 = fadeOut("king", a1, seconds, 3);
  }

  // return to menu
  if (totalTime > 4.5) {
    totalTime = 0;
    Mix_HaltMusic();
    engine->setState("menu");
  }
}

LoseState::~LoseState() {}
