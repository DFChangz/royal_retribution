/*
 * highscore_state.cpp
 */

#include "highscore_state.h"

HighscoreState::HighscoreState(Engine* eng, ErrorHandler* eHandler)
  :  State(eng, eHandler) {

    setup();
    load();
}

void HighscoreState::setup() {

  // title
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
  WIDTH / 2, HEIGHT, 40, "HighScore"));
  // read file
  file.open(SCORE_FILENAME);
  if (file.is_open()) {
    while (std::getline(file, line)) {
      scores.push_back(stoi(line));
    }
    file.close();
  }
  // order scores
  std::sort(scores.begin(), scores.end(), std::greater<int>());
  // create texts
  for (int i = 0; i < TOTAL; i++) {
    images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images[i]->getDestRect()->y+60, 30, std::to_string(scores[i])));
  }
}

void HighscoreState::load() {
  State::load();

  // center the text
  for (Image* words: images) {
    int x = WIDTH / 2 - words->getDestRect()->w / 2;
    int y = words->getDestRect()->y;
    words->setPosition(x, y);
    words->velocityY = -200;
  }
}

void HighscoreState::update(double seconds){
  State::update(seconds);

  if(images[0]->getDestRect()->y <= 0) {
    for(Image* words: images){
      words->velocityY = 0;
    }
  }
  // green
  SDL_SetTextureColorMod(images[0]->getTexture(), 0, 180, 60);
  // blue
  for (int i = 1; i < TOTAL + 1; i++) {
    SDL_SetTextureColorMod(images[i]->getTexture(), 0, 80, 190);
  }
} 

HighscoreState::~HighscoreState(){}
