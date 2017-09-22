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
    images[0]->getDestRect()->y + 80, 40, std::to_string(scores) ));

  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[1]->getDestRect()->y + 80, 40, "Credits" ));
}

void HighscoreState::load() {
  State::load();

  int x = WIDTH / 2 - images[0]->getDestRect()->w / 2;
  int y = images[0]->getDestRect()->y;
  images[0]->setPosition(x, y);

  x = WIDTH / 2 - images[1]->getDestRect()->w / 2;
  y = images[1]->getDestRect()->y;
  images[1]->setPosition(x, y);

  x = WIDTH / 2 - images[2]->getDestRect()->w / 2;
  y = images[2]->getDestRect()->y;
  images[2]->setPosition(x, y);

  images[0]->velocityY = -80;
  images[1]->velocityY = -80;
  images[2]->velocityY = -80;
}

void HighscoreState::update(double seconds){
  State::update(seconds);

  if(images[0]->getDestRect()->y <= 0) {
    images[0]->velocityY = 0;
    images[1]->velocityY = 0;
    images[2]->velocityY = 0;
  }

} 

HighscoreState::~HighscoreState(){}
