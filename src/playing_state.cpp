#include "playing_state.h"

PlayingState::PlayingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

	setup();
}

void PlayingState::setup() {
  images.push_back(new Image(engine->renderer, BG_FILENAME, errorHandler));
  images.push_back(new Character(engine->renderer, CHARACTER_FILENAME,
    errorHandler, 0, 0, &eventHandler, &audioHandler));

  for (int i = 0; i < 5; i++) {
    images.push_back(new Enemy(engine->renderer, ENEMY_FILENAME,
      errorHandler, rand() % WIDTH, rand() % HEIGHT, rand() % 100 + 1,
      rand() % 100 + 1));
  }
}

PlayingState::~PlayingState() {}
