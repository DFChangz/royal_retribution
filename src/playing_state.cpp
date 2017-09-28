/*
 * playing_state.cpp
 */

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
    images.push_back(new Enemy(engine->renderer, E_C_FILENAME, errorHandler,
      16, 25, rand() % WIDTH - 16, rand() % HEIGHT - 25,
      rand() % 100 + 1, rand() % 100 + 1));
  }
  //Current Score
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH - 100, 2, 16, "SCORE = " + std::to_string(engine->score)));  
  // FPS Counter
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    2, 2, 16, "FPS: "));
  // Stairs
  images.push_back(new Sprite(engine->renderer, STAIRS_FILENAME, errorHandler,
    0, 0, false));
}

void PlayingState::load() {
  State::load();

  // change stairs size
  images[9]->getDestRect()->w = 50;
  images[9]->getDestRect()->h = 50;
  // set pos of stairs img
  images[9]->setPosition(WIDTH - 50, HEIGHT - 50);
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
    delete images[7];
    images[7] = new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH - 100, 2, 16, "SCORE = " + std::to_string(engine->score));  
    images[7]->load();
    currentScore = engine->score;
  }

  // changes state to lose/win
  State::update(seconds);
  if (static_cast<Character*>(images[1])->hearts == 0) {
    engine->setState("lose");
  }
  if (images[1]->pos_x > WIDTH - images[9]->getDestRect()->w
      && images[1]->pos_y > HEIGHT - images[9]->getDestRect()->h) {
    engine->setState("win");
  }
}

PlayingState::~PlayingState() {}
