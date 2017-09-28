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
  // BG 0
  images.push_back(new Image(engine->renderer, BG_FILENAME, errorHandler));
  // Stairs 1
  images.push_back(new Sprite(engine->renderer, STAIRS_FILENAME, errorHandler,
    WIDTH - 83, HEIGHT - 82, false));
  // Player 2
  images.push_back(new Character(engine->renderer, E_C_FILENAME,
    errorHandler, 16, 25, WIDTH/2, 0, &eventHandler, &audioHandler));
  // Enemies 3 - 12
  for (int i = 0; i < 10; i++) {
    if (i%2)
      images.push_back(new Enemy(engine->renderer, E_C_FILENAME, errorHandler,
        16, 25, (i+1) * WIDTH/11, (i+1) * HEIGHT/11, 200, 0));
    else
      images.push_back(new Enemy(engine->renderer, E_C_FILENAME, errorHandler,
        16, 25, (i+1) * WIDTH/11, (i+1) * HEIGHT/11, 0, 200));
  }
  // Score 13
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 
    WIDTH - 100, 2, 16, "SCORE = " + std::to_string(engine->score)));  
  // Heart types 14, 15, 16
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 120, 34, false));
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 80, 34, false));
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 40, 34, false));
  // FPS Counter 17
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    2, 2, 16, "FPS: "));
}

void PlayingState::load() {
  State::load();

  // set size of stairs
  images[1]->getDestRect()->w = 90;
  images[1]->getDestRect()->h = 90;
}

void PlayingState::update(double seconds) {
  State::update(seconds);

  timer += seconds;

  // update FPS Display
  if (timer > 1) {
    delete images.back();
    images.pop_back();
    images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
      2, 2, 16, "FPS: " + std::to_string((int)(1/seconds))));
    images.back()->load();

    timer = 0;
  }
  // update Score
  if(currentScore != engine->score){
    delete images[13];
    images[13] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
      WIDTH - 100, 2, 16, "SCORE = " + std::to_string(engine->score));  
    images[13]->load();
    currentScore = engine->score;
  }
  // updates Health
  switch(static_cast<Character*>(images[2])->hearts){  
    case 6:
      static_cast<Sprite*>(images[16])->setSrcRect(0, 0, 32, 32);
      break;
    case 5:
      static_cast<Sprite*>(images[16])->setSrcRect(40, 0, 32, 32);
      break;
    case 4:
      static_cast<Sprite*>(images[16])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[15])->setSrcRect(0, 0, 32, 32);
      break;
    case 3:
      static_cast<Sprite*>(images[15])->setSrcRect(40, 0, 32, 32);
      break;
    case 2:
      static_cast<Sprite*>(images[15])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[14])->setSrcRect(0, 0, 32, 32);
      break;
    case 1:
      static_cast<Sprite*>(images[14])->setSrcRect(40, 0, 32, 32);
      break;
  }
  // changes state to Lose/Win
  if (static_cast<Character*>(images[2])->hearts == 0) {
    engine->setState("lose");
  }
  if (images[2]->pos_x > WIDTH - 75 && images[2]->pos_y > HEIGHT - 75) {
    engine->setState("win");
  }
}

PlayingState::~PlayingState() {}
