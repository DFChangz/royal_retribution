#include "playing_state.h"

PlayingState::PlayingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

	setup();
  load();
}

void PlayingState::setup() {
  images.push_back(new Image(engine->renderer, BG_FILENAME, errorHandler));
  images.push_back(new Character(engine->renderer, E_C_FILENAME,
    errorHandler, 16, 25, 0, 0, &eventHandler, &audioHandler));

  for (int i = 0; i < 5; i++) {
    images.push_back(new Enemy(engine->renderer, E_C_FILENAME,
      errorHandler, 16, 25, rand() % WIDTH - 16, rand() % HEIGHT - 25, rand() % 100 + 1,
      rand() % 100 + 1));
  }
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 2, 2, 16, "FPS: "));
}

void PlayingState::update(double seconds) {
  timer += seconds;

  if (timer > 1) {
    delete images.back();
    images.pop_back();
    images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 2, 2, 16, "FPS: " + std::to_string((int)(1/seconds))));
    images.back()->load();

    timer = 0;
  }
  State::update(seconds);
}

PlayingState::~PlayingState() {}
