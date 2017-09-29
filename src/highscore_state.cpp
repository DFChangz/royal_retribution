/*
 * highscore_state.cpp
 */

#include "highscore_state.h"

HighscoreState::HighscoreState(Engine* eng, ErrorHandler* eHandler)
  :  State(eng, eHandler) {
     setup();

    load();
}
void HighscoreState::setup(){
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 
    WIDTH / 2, HEIGHT, 40, "HighScore" ));
  
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[0]->getDestRect()->y + 60, 40, std::to_string(scores) ));

}

void HighscoreState::load() {
  State::load();

  int x = WIDTH / 2 - images[0]->getDestRect()->w / 2;
  int y = images[0]->getDestRect()->y;
  images[0]->setPosition(x, y);

  x = WIDTH / 2 - images[1]->getDestRect()->w / 2;
  y = images[1]->getDestRect()->y;
  images[1]->setPosition(x, y);


  for(Image* words: images) {
    words->velocityY = -80;
  }
}

void HighscoreState::update(double seconds){
  State::update(seconds);

  if(images[1]->getDestRect()->y <= 0 - images[1]->getDestRect()->h) {
    for(Image* words: images){
      words->velocityY = 0;
    }
    engine->setState("credits");
  }

} 

HighscoreState::~HighscoreState(){}
