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

  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[1]->getDestRect()->y + 60, 40, "Credits" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[2]->getDestRect()->y + 60, 40, "Programmers" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[3]->getDestRect()->y + 60, 30, "Idean Labib" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[4]->getDestRect()->y + 60, 30, "Tae Jin Kim" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[5]->getDestRect()->y + 60, 30, "Billy Carrington" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[6]->getDestRect()->y + 60, 40, " Graphics" ));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, WIDTH / 2, 
    images[7]->getDestRect()->y + 60, 30, "Derek Chang" ));
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

  x = WIDTH / 2 - images[3]->getDestRect()->w / 2;
  y = images[3]->getDestRect()->y;
  images[3]->setPosition(x, y);

  x = WIDTH / 2 - images[4]->getDestRect()->w / 2;
  y = images[4]->getDestRect()->y;
  images[4]->setPosition(x, y);

  x = WIDTH / 2 - images[5]->getDestRect()->w / 2;
  y = images[5]->getDestRect()->y;
  images[5]->setPosition(x, y);

  x = WIDTH / 2 - images[6]->getDestRect()->w / 2;
  y = images[6]->getDestRect()->y;
  images[6]->setPosition(x, y);

  x = WIDTH / 2 - images[7]->getDestRect()->w / 2;
  y = images[7]->getDestRect()->y;
  images[7]->setPosition(x, y);

  x = WIDTH / 2 - images[8]->getDestRect()->w / 2;
  y = images[8]->getDestRect()->y;
  images[8]->setPosition(x, y);

  for(Image* words: images) {
    words->velocityY = -80;
  }
}

void HighscoreState::update(double seconds){
  State::update(seconds);

  if(images[0]->getDestRect()->y <= 0) {
    for(Image* words: images){
      words->velocityY = 0;
    }
    SDL_SetTextureColorMod(images[3]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images[7]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images[4]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images[5]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images[6]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images[8]->getTexture(), 0, 80, 190);
  }

} 

HighscoreState::~HighscoreState(){}
