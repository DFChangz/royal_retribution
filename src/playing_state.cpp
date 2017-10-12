/*
 * playing_state.cpp
 */

#include "playing_state.h"

PlayingState::PlayingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_0, TILES_TXT);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_0_ADD);

  setup();
  load();
}

void PlayingState::setup() {
  // Stairs 0
  images.push_back(new Sprite(engine->renderer, STAIRS_FILENAME, errorHandler,
    map->width/2 - 45, map->height - 150, false));
  // Player 1
  images.push_back(new Character(engine->renderer, E_C_FILENAME, errorHandler,
    16, 25, 80, 80, &eventHandler, &audioHandler, this));
  camera.setCharacter(static_cast<Character*>(images[1]));
  // Enemies
  std::ifstream file(LEVEL_0_E);
  int x = -1;
  int y = -1;
  while ((file >> y) && y != -1 && (file >> x) && x != -1) {
    images.push_back(new Enemy(engine->renderer, E_C_FILENAME, errorHandler,
      16, 25, (x-1) * TILE_DIM, (y-1) * TILE_DIM, 0, 150));
    num_enemies++;
  }
  file.close();

  // Lights
  map->pushLights(images);
  num_lights = images.size() - num_enemies - 2;
  // Black 2
  images.push_back(new Sprite(engine->renderer, BLACK_PIXEL, errorHandler, 0,
    0, false));

  // Score 3
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 
    WIDTH - 114, 2, 16, "SCORE = " + std::to_string(engine->score)));  
  // Heart types 4-6
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 120, 34, false, true));
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 80, 34, false, true));
  images.push_back(new Sprite(engine->renderer, HEART,
    errorHandler, 32, 32, WIDTH - 40, 34, false, true));
  // Stamina 7-8
  images.push_back(new Sprite(engine->renderer, STA_BOX,
    errorHandler, 0, 34, false, true));
  images.push_back(new Sprite(engine->renderer, STA_BAR,
    errorHandler, 2, 38, false, true));
  // FPS Counter 9
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler,
    2, 2, 16, "FPS: "));
}

void PlayingState::load() {
  State::load();

  // set size of stairs & stam
  images[0]->getDestRect()->h = 90;
  images[0]->getDestRect()->w = 90;
  images[7 + num_enemies + num_lights]->getDestRect()->h = 32;
  images[7 + num_enemies + num_lights]->getDestRect()->w = 100;
  images[8 + num_enemies + num_lights]->getDestRect()->h = 24;
  images[8 + num_enemies + num_lights]->getDestRect()->w = 92;

  for (int i = num_enemies + 2; i < num_enemies + num_lights + 2;
        i++) {

    images[i]->getDestRect()->w = TILE_DIM * 5;
    images[i]->getDestRect()->h = TILE_DIM * 5;

    SDL_SetTextureBlendMode(images[i]->getTexture(), SDL_BLENDMODE_ADD);
    if (SDL_SetTextureAlphaMod(images[i]->getTexture(), 80) < 0)
      errorHandler->quit(__func__, SDL_GetError());
  }

  images[num_enemies + num_lights + 2]->getDestRect()->w = map->width;
  images[num_enemies + num_lights + 2]->getDestRect()->h = map->height;

  SDL_SetTextureBlendMode(images[num_enemies + num_lights + 2]->getTexture(),
    SDL_BLENDMODE_BLEND);
  if (SDL_SetTextureAlphaMod(images[num_enemies + num_lights + 2]->getTexture(),
    200) < 0) {

    errorHandler->quit(__func__, SDL_GetError());
  }
}

void PlayingState::update(double seconds) {
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
    delete images[num_enemies + num_lights + 3];
    images[num_enemies + num_lights + 3] = new Text(engine->renderer, FONT_FILENAME,
      errorHandler, WIDTH - 114, 2, 16, "SCORE = " +
      std::to_string(engine->score));  
    images[num_enemies + num_lights + 3]->load();
    currentScore = engine->score;
  }
  // updates Health
  switch(static_cast<Character*>(images[1])->hearts) {
    case 6:
      static_cast<Sprite*>(images[num_enemies + num_lights + 6])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 5])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 4])->setSrcRect(0, 0, 32, 32);
      break;
    case 5:
      static_cast<Sprite*>(images[num_enemies + num_lights + 6])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 5])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 4])->setSrcRect(0, 0, 32, 32);
      break;
    case 4:
      static_cast<Sprite*>(images[num_enemies + num_lights + 6])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 5])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 4])->setSrcRect(0, 0, 32, 32);
      break;
    case 3:
      static_cast<Sprite*>(images[num_enemies + num_lights + 6])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 5])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 4])->setSrcRect(0, 0, 32, 32);
      break;
    case 2:
      static_cast<Sprite*>(images[num_enemies + num_lights + 6])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 5])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 4])->setSrcRect(0, 0, 32, 32);
      break;
    case 1:
      static_cast<Sprite*>(images[num_enemies + num_lights + 6])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 5])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[num_enemies + num_lights + 4])->setSrcRect(40, 0, 32, 32);
      break;
  }
  // update stamina
  updateSta();
  
  // enemy follows king if conditions met
  checkFollow();
  enemyFollow();
  
  State::update(seconds);

  // changes state to Lose
  if (static_cast<Character*>(images[1])->hearts <= 0) {
    engine->setState("lose");
  }
  // changes state to Win
  if (images[1]->pos_x > map->width/2 - 45
      && images[1]->pos_x < map->width/2 + 45
      && images[1]->pos_y > map->height - 150)
  {
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file << std::to_string(engine->score) << std::endl;
    file.close();
    engine->setState("win");
  }
//  audioHandler.setVolume(engine->volume);
}

void PlayingState::checkFollow() {
  int borderX = images[1]->pos_x + images[1]->getDestRect()->w + 100;
  int borderY = images[1]->pos_y + images[1]->getDestRect()->h + 100;
  for (int i = 2; i < num_enemies + 2; i++) {
    if (!static_cast<Character*>(images[1])->running
        && images[i]->pos_x
        >= images[1]->pos_x - 100 - images[i]->getDestRect()->w
        && images[i]->pos_y
        >= images[1]->pos_y - 100 - images[i]->getDestRect()->h
        && images[i]->pos_x <= borderX && images[i]->pos_y <= borderY)
    {
      static_cast<Enemy*>(images[i])->following = true;
    }
  }
}
      
void PlayingState::enemyFollow() {
  for (int i = 2; i < num_enemies + 2; i++) {
    if (static_cast<Enemy*>(images[i])->following) {
      // edit x velocity
      if (images[i]->pos_x < images[1]->pos_x-images[i]->getDestRect()->w+1) {
        images[i]->velocityX = 100;
      } else
      if (images[i]->pos_x > images[1]->pos_x+images[1]->getDestRect()->w-1) {
        images[i]->velocityX = -100;
      } else {
        images[i]->velocityX = 0;
      }
      // edit y velocity
      if (images[i]->pos_y + images[i]->getDestRect()->h / 2
           < images[1]->pos_y + images[1]->getDestRect()->h / 2) {
        images[i]->velocityY = 100;
      } else
      if (images[i]->pos_y + images[i]->getDestRect()->h / 2 
          > images[1]->pos_y + images[1]->getDestRect()->h / 2) {
        images[i]->velocityY = -100;
      } else {
        images[i]->velocityY = 0;
      } 
    }
  }
}

// inc/dec sta
void PlayingState::updateSta() {
  int w = int(static_cast<Character*>(images[1])->sta * 96);  
  images[num_enemies + num_lights + 8]->getDestRect()->w = w;
  std::cout << static_cast<Character*>(images[1])->sta << " " <<  w << "\n";
}

PlayingState::~PlayingState() {}
