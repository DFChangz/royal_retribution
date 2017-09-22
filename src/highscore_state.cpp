#include "highscore_state.h"

HighscoreState::HighscoreState(Engine* eng, ErrorHandler* eHandler)
  :  State(eng, eHandler) {
     setup();

}
void HighscoreState::setup(){
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 
    WIDTH / 2, HEIGHT, 40, "HighScore" ));
  
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[0]->getDestRect()->y + 80, 40, std::to_string(scores) ));

  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[1]->getDestRect()->y + 80, 40, "Credits" ));
}

void HighscoreState::update(double){
  images[0]->getDestRect()->y = images[0]->getDestRect()->y - images[0]->getDestRect()->h / 16;
  images[1]->getDestRect()->y = images[1]->getDestRect()->y - images[1]->getDestRect()->h / 16;
  images[2]->getDestRect()->y = images[2]->getDestRect()->y - images[2]->getDestRect()->h / 16;

  if(images[0]->getDestRect()->y <= 0) {
    images[0]->getDestRect()->y = 0;
    images[1]->getDestRect()->y = 80;
    images[2]->getDestRect()->y = 160;
  }

} 

HighscoreState::~HighscoreState(){}
