/*
 * playing_state.cpp
 */

#include "playing_state.h"

PlayingState::PlayingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler,
    "test_map.txt", "test_textures.txt");

  setup();
  load();
}

void PlayingState::setup() {
  // Stairs 0
  images.push_back(new Sprite(engine->renderer, STAIRS_FILENAME, errorHandler,
    map->width - 83, map->height - 82, false));
  // Player 1
  images.push_back(new Character(engine->renderer, E_C_FILENAME,
    errorHandler, 16, 25, 80, 80, &eventHandler, &audioHandler, this));
  camera.setCharacter(static_cast<Character*>(images[1]));
  // Enemies 2 - 11
  for (int i = 0; i < 10; i++) {
    if (i%2)
      images.push_back(new Enemy(engine->renderer, E_C_FILENAME, errorHandler,
        16, 25, i * (map->width - TILE_DIM * 6)/11 + TILE_DIM * 3,
        i * (map->height - TILE_DIM * 6)/11 + TILE_DIM * 3, 250, 0));
    else
      images.push_back(new Enemy(engine->renderer, E_C_FILENAME, errorHandler,
        16, 25, i * (map->width - TILE_DIM * 6)/11 + TILE_DIM * 3,
        i * (map->height - TILE_DIM * 6)/11 + TILE_DIM * 3, 0, 150));
  }
  // Score 12
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 
    WIDTH - 120, 2, 16, "SCORE = " + std::to_string(engine->score)));  
  // Heart types 13, 14, 15
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 120, 34, false, true));
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 80, 34, false, true));
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 40, 34, false, true));
  // FPS Counter 16
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    2, 2, 16, "FPS: "));
}

void PlayingState::load() {
  State::load();

  // set size of stairs
  images[0]->getDestRect()->w = 90;
  images[0]->getDestRect()->h = 90;
}

void PlayingState::update(double seconds) {
  State::update(seconds);

  timer += seconds;

  if(Mix_PausedMusic() == 1){audioHandler.play("theme");}
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
    delete images[12];
    images[12] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
      WIDTH - 100, 2, 16, "SCORE = " + std::to_string(engine->score));  
    images[12]->load();
    currentScore = engine->score;
  }
  // updates Health
  switch(static_cast<Character*>(images[1])->hearts) {
    case 6:
      static_cast<Sprite*>(images[15])->setSrcRect(0, 0, 32, 32);
      break;
    case 5:
      static_cast<Sprite*>(images[15])->setSrcRect(40, 0, 32, 32);
      break;
    case 4:
      static_cast<Sprite*>(images[15])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[14])->setSrcRect(0, 0, 32, 32);
      break;
    case 3:
      static_cast<Sprite*>(images[14])->setSrcRect(40, 0, 32, 32);
      break;
    case 2:
      static_cast<Sprite*>(images[14])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[13])->setSrcRect(0, 0, 32, 32);
      break;
    case 1:
      static_cast<Sprite*>(images[13])->setSrcRect(40, 0, 32, 32);
      break;
  }
  // enemy follows king if conditions met
  checkFollow();
  enemyFollow();
  std::cout << images[2]->velocityX << std::endl;
  // changes state to Lose/Win
  if (static_cast<Character*>(images[1])->hearts == 0) {
    engine->setState("lose");
  }
  if (images[1]->pos_x > map->width - 75 && images[1]->pos_y > map->height - 75) {
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file << std::to_string(currentScore) << std::endl;
    file.close();
    engine->setState("win");
  }
  audioHandler.setVolume(engine->volume);
}

void PlayingState::checkFollow() {
  int borderX = images[1]->pos_x + images[1]->getDestRect()->w + 100;
  int borderY = images[1]->pos_y + images[1]->getDestRect()->h + 100;
  for (int i = 2; i < 12; i++) {
    if (images[i]->pos_x <= borderX && images[i]->pos_y <= borderY
        && images[i]->pos_x
        >= images[1]->pos_x - 100 - images[i]->getDestRect()->w
        && images[i]->pos_y
        >= images[1]->pos_y - 100 - images[i]->getDestRect()->h    )
    {
      static_cast<Enemy*>(images[i])->following = true;
    }
  }
}
      
void PlayingState::enemyFollow() {
  for (int i = 2; i < 12; i++) {
    if (static_cast<Enemy*>(images[i])->following) {
      if (images[i]->pos_x < images[1]->pos_x - images[i]->getDestRect()->w) {
        images[i]->velocityX = 100;
      } else
      if (images[i]->pos_x > images[1]->pos_x + images[1]->getDestRect()->w) {
        images[i]->velocityX = -100;
      } else {
        images[i]->velocityX = 0;
      }
      if (images[i]->pos_y < images[1]->pos_y - images[i]->getDestRect()->h) {
        images[i]->velocityY = 100;
      } else
      if (images[i]->pos_y > images[1]->pos_x + images[1]->getDestRect()->h) {
        images[i]->velocityY = -100;
      } else {
        images[i]->velocityY = 0;
      } 
    }
  }
}

PlayingState::~PlayingState() {}
