/*
 * highscore_state.cpp
 */

#include "highscore_state.h"

HighscoreState::HighscoreState(Engine* eng, ErrorHandler* eHandler)
  :  State(eng, eHandler) {
    std::cout << "start HS\n";
    setup();
    std::cout << "finish setup\n";
    load();
    std::cout << "finish load\n";
}

void HighscoreState::setup() {
  // title
  images["title"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, "HighScore");
  std::cout << "make title\n";
  // read file
  file.open(SCORE_FILENAME);
  if (file.is_open()) {
    while (std::getline(file, line)) {
      scores.push_back(stoi(line));
    }
    file.close();
  }
  std::cout << "read file\n";
  // order scores
  std::sort(scores.begin(), scores.end(), std::greater<int>());
  std::cout << "order scores\n";
  // create texts
  for (int i = 0; i < TOTAL; i++) {
    std::string prev = "title";
    std::string s = "score_" + i;
    if (i > 0) prev = "score_" + i-1;
    images[s] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
      0, images[prev]->getDestRect()->y+60, 30, std::to_string(scores[i]));
  }
  std::cout << "add all scores\n";
}

void HighscoreState::load() {
  State::load();
  // center(x) of text and fade out
  for (it = images.begin(); it != images.end(); it++) {
    SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
    int x = WIDTH / 2 - it->second->getDestRect()->w / 2;
    int y = it->second->getDestRect()->y;
    it->second->setPosition(x, y);
  }
}

void HighscoreState::update(double seconds) {
  State::update(seconds);

  // green
  SDL_SetTextureColorMod(images["title"]->getTexture(), 0, 180, 60);
  // blue
  for (int i = 0; i < TOTAL; i++) {
    std::string s = "score_" + i;
    SDL_SetTextureColorMod(images[s]->getTexture(), 0, 80, 190);
  }
  // fade in
  a0 = fadeIn("title", a0, seconds, 4);
  a1 = fadeIn("score_0", a1, seconds, 4);
  a2 = fadeIn("score_1", a2, seconds, 4);
  a3 = fadeIn("score_2", a3, seconds, 4);
  a4 = fadeIn("score_3", a4, seconds, 4);
  a5 = fadeIn("score_4", a5, seconds, 4);
  a6 = fadeIn("score_5", a6, seconds, 4);
  a7 = fadeIn("score_6", a7, seconds, 4);
  a8 = fadeIn("score_7", a8, seconds, 4);
  a9 = fadeIn("score_8", a9, seconds, 4);
  a10 = fadeIn("score_9", a10, seconds, 4);
} 

HighscoreState::~HighscoreState(){}
