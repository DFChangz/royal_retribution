/*
 * playing_state.cpp
 */

#include "playing_state.h"

PlayingState::PlayingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, &camera, errorHandler, "test_map.txt", "test_textures.txt");

  setup();
  load();
}

void PlayingState::setup() {
  images.push_back(new Character(engine->renderer, E_C_FILENAME,
    errorHandler, 16, 25, 0, 0, &eventHandler, &audioHandler));
  camera.setCharacter(static_cast<Character*>(images[0]));

  for (int i = 0; i < 5; i++) {
    images.push_back(new Enemy(engine->renderer, E_C_FILENAME, errorHandler,
      16, 25, rand() % WIDTH - 16, rand() % HEIGHT - 25,
      rand() % 100 + 1, rand() % 100 + 1));
  }
  //Current Score
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH - 100, 2, 16, "SCORE = " + std::to_string(engine->score)));  
  // FPS Counter
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    2, 2, 16, "FPS: "));
}

void PlayingState::update(double seconds) {
  timer += seconds;

  /* Every second, remove the FPS Counter image and create a new one with the
  updated FPS. */
  if (timer > 1) {
    delete images.back();
    images.pop_back();
    images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
      2, 2, 16, "FPS: " + std::to_string((int)(1/seconds))));
    images.back()->load();

    timer = 0;
  }
  if(currentScore != engine->score){
    delete images[6];
    images[6] = new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH - 100, 2, 16, "SCORE = " + std::to_string(engine->score));  
    images[6]->load();
    currentScore = engine->score;
  }

  State::update(seconds);
  if (static_cast<Character*>(images[0])->hearts == 0) {
    engine->setState("lose");
  }
  if (images[0]->pos_x == 0 && images[0]->pos_y == 0) {
    engine->setState("win");
  }
}

PlayingState::~PlayingState() {}
