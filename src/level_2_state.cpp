/*
 * level_2_state.cpp
 */

#include "level_2_state.h"

Level_2_State::Level_2_State(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_2, TILES_TXT,
    &collisionDetector);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_2_ADD);

  setup();
  load();
}

void Level_2_State::setup() {
  // Stairs 
  images[ele+"stairs"] = new Sprite(engine->renderer, STAIRS_FILENAME,
    errorHandler, map->width/2 - 45, map->height - 150, false);
  // Hole
  images[ele+"hole"] = new Sprite(engine->renderer, BLACK_PIXEL,
    errorHandler, 0, 0, false);
  // King
  if (king != nullptr) {
    images[ppl+"king"] = king;
  } else {
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, 128, 358, &eventHandler, &audioHandler, this);
  }
  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, 0, 0, static_cast<Sprite*>(images[ppl+"king"]), &eventHandler, &audioHandler, this);
  // Enemies
  std::ifstream file(LEVEL_2_E);
  int x = -1;
  int y = -1;
  while ((file >> y) && y != -1 && (file >> x) && x != -1) {
    std::string s = ppl+"enemy_"+std::to_string(num_enemies);
    images[s] = new Enemy(engine->renderer, ANI_FILENAME, errorHandler,
      16, 25, (x-1) * TILE_DIM, (y-1) * TILE_DIM, 0, 150);
    num_enemies++;
  }
  file.close();

  // Lights
  num_lights = map->pushLights(images);
  // Black
  images[add+"black"] = new Sprite(engine->renderer, BLACK_PIXEL, errorHandler,
    0, 0, false);

  // Skip Pan Test
  images[top+"skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH/2-170, HEIGHT-35, 30, "Press [m] to Skip Pan", ROYAL_GOLD);

  // Score
  images[top+"score"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH - 114, 2, 16, "SCORE = " + std::to_string(engine->score));
  // Level
  images[top+"level"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH/2-48, 2, 16, "KING LEVEL "); 
  // Heart types
  images[top+"heart_4"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 160, 34, false, true);
  images[top+"heart_3"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 120, 34, false, true);
  images[top+"heart_2"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 80, 34, false, true);
  images[top+"heart_1"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 40, 34, false, true);
  // Stamina
  images[top+"sta_box"] = new Sprite(engine->renderer, BAR_BOX, errorHandler,
    0, 34, false, true);
  images[top+"sta_bar"] = new Sprite(engine->renderer, STA_BAR, errorHandler,
    2, 38, false, true);
  // Experience
  images[top+"exp_box"] = new Sprite(engine->renderer, BAR_BOX, errorHandler,
    WIDTH/2 - 75, 34, false, true);
  images[top+"exp_bar"] = new Sprite(engine->renderer, EXP_BAR, errorHandler,
    WIDTH/2 - 72, 38, false, true);
  // set key and coin pos
  double keyPosX = 0.0;
  double keyPosY = 0.0;
  double coinPosX = 0.0;
  double coinPosY = 0.0;
  Sprite *C1 = nullptr;
  Sprite *C2 = nullptr;
  for(auto tile : map->additions){
    if(tile.image->isChest()){
      if(keyPosX == 0.0 && keyPosY == 0.0){
        keyPosX = tile.image->pos_x;
        keyPosY = tile.image->pos_y;
        C1 = tile.image;
      } else {
        coinPosX = tile.image->pos_x;
        coinPosY = tile.image->pos_y;
        C2 = tile.image;
      }
    }
  }
  // add key
  images[add+"key"] = new Pickup(engine->renderer, KEY, errorHandler,
    32, 32, keyPosX, keyPosY, false, false, keyNum);
  static_cast<Sprite*>(images[add+"key"])->setPair(C1);
  // add coin
  images[add+"coin"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPosX, coinPosY, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin"])->setPair(C2);
  //instuctions
  images[top+"dkInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "You got a key. It opens a special door. Press 'r' to clear text");
  images[top+"hInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "You fell down a hole. You are now going to the previous floor. Press 'r' to clear text");
  images[top+"tInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU ARE TRAPPED, KILL AN ENEMY TO ESCAPE! press 'r' to clear text ");
  images[top+"cInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "You opened a chest the item is now in your inventory in the upper left corner. press 'r' to clear text ");
  // FPS Counter 
  images[add+"fps"] = new Text(engine->renderer, FONT_FILENAME,  errorHandler,
    2, 2, 16, "FPS: ");
}

void Level_2_State::update(double seconds) {
  PlayingState::update(seconds);

  // changes state to level 3
  if (images[ppl+"king"]->pos_x < map->width/2 + 45
      && images[ppl+"king"]->pos_x + images[ppl+"king"]->getDestRect()->w
        > map->width/2 - 45
      && images[ppl+"king"]->pos_y + images[ppl+"king"]->getDestRect()->h
        > map->height - 150)
  {
    images[ppl+"king"]->pos_x = static_cast<Character*>(images[ppl+"king"])->startingX;
    images[ppl+"king"]->pos_y = static_cast<Character*>(images[ppl+"king"])->startingY;
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    Character::currFloor = 3;
    if (Character::highestFloor == 2) Character::highestFloor = 3;
    engine->setState("level_3");
  }
  // go to floor 3 w/ '2'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    images[ppl+"king"]->pos_x = static_cast<Character*>(images[ppl+"king"])->startingX;
    images[ppl+"king"]->pos_y = static_cast<Character*>(images[ppl+"king"])->startingY;
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    static_cast<Character*>(images[ppl+"king"])->dir = "down";
    king = images[ppl+"king"];
    Character::currFloor = 3;
    if (Character::highestFloor == 2) Character::highestFloor = 3;
    engine->setState("level_3");
   }, SDLK_2);
  // go to floor 1 w/ '1'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    Character::currFloor = 1;
    engine->setState("playing");
   }, SDLK_1);
}

Level_2_State::~Level_2_State() {}
