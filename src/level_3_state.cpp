/*
 * level_3_state.cpp
 */

#include "level_3_state.h"

Level_3_State::Level_3_State(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_3, TILES_TXT,
    &collisionDetector);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_3_ADD);
  setup();
  load();
}

void Level_3_State::setup() {
  // Stairs 
  images[ele+"stairs"] = new Sprite(engine->renderer, STAIRS_FILENAME,
    errorHandler, map->width/2 - 45, map->height - 150, false);
  camera.setPosition(images[ele+"stairs"]);
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
  std::ifstream file(LEVEL_3_E);
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
  // FPS Counter 
  images[add+"fps"] = new Text(engine->renderer, FONT_FILENAME,  errorHandler,
    2, 2, 16, "FPS: ");
}

void Level_3_State::load() {
  State::load();
  // set size of stairs & stam & exp
  images[ele+"stairs"]->getDestRect()->h = 90;
  images[ele+"stairs"]->getDestRect()->w = 90;
  images[top+"sta_box"]->getDestRect()->h = 32;
  images[top+"sta_box"]->getDestRect()->w = 150;
  images[top+"sta_bar"]->getDestRect()->h = 24;
  images[top+"sta_bar"]->getDestRect()->w = 146;
  images[top+"exp_box"]->getDestRect()->h = 32;
  images[top+"exp_box"]->getDestRect()->w = 150;
  images[top+"exp_bar"]->getDestRect()->h = 24;
  images[top+"exp_bar"]->getDestRect()->w = 148;

  for (int i = 0; i < num_lights; i++) {
    std::string s = add+"light_"+std::to_string(i);

    images[s]->getDestRect()->w = TILE_DIM;
    images[s]->getDestRect()->h = TILE_DIM;

    SDL_SetTextureBlendMode(images[s]->getTexture(),SDL_BLENDMODE_ADD);
    if (SDL_SetTextureAlphaMod(images[s]->getTexture(), 80) < 0)
      errorHandler->quit(__func__, SDL_GetError());
  }

  images[add+"black"]->getDestRect()->w = map->width;
  images[add+"black"]->getDestRect()->h = map->height;

  SDL_SetTextureBlendMode(images[add+"black"]
    ->getTexture(), SDL_BLENDMODE_BLEND);
  if (SDL_SetTextureAlphaMod(images[add+"black"]->getTexture(), 150) < 0) {
    errorHandler->quit(__func__, SDL_GetError());
  }
}

void Level_3_State::update(double seconds) {
  timer += seconds;

  if (!skipPan && !camera.pan(images[ppl+"king"], seconds)) {
    static_cast<Character*>(images[ppl+"king"])->frozen = true;
    eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
      skipPan = true; }, SDLK_m);
  } else {
    static_cast<Character*>(images[ppl+"king"])->frozen = false;
    camera.setCharacter(static_cast<Character*>(images[ppl+"king"]));
  }

  int kingLevel = static_cast<Character*>(images[ppl+"king"])->level;

  if(Mix_PausedMusic() == 1){audioHandler.play("theme");}
  // update FPS Display
  if (timer > 1) {
    delete images[top+"fps"];
    images[top+"fps"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
      2, 2, 16, "FPS: " + std::to_string((int)(1/seconds)));
    images[top+"fps"]->load();
    timer = 0;
  }
  // update Level
  if (currentLevel != kingLevel) {
    delete images[top+"level"];
    images[top+"level"] = new Text(engine->renderer, FONT_FILENAME,
      errorHandler, WIDTH/2-48, 2, 16, "KING LEVEL "+std::to_string(kingLevel));
    images[top+"level"]->load();
    currentLevel = kingLevel;
  }
  // update Score
  if (currentScore != engine->score) {
    delete images[top+"score"];
    images[top+"score"] = new Text(engine->renderer, FONT_FILENAME,
      errorHandler, WIDTH-114, 2, 16, "SCORE = "+std::to_string(engine->score));
    images[top+"score"]->load();
    currentScore = engine->score;
  }

  // update stamina & experience
  updateSta();
  updateExp();
  
  // enemy follows king if conditions met
  checkFollow();
  enemyFollow();

  SDL_SetTextureAlphaMod(images[add+"key"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"coin"]->getTexture(), 0);

  int x = 0;  
// set up inventory display
  for(Pickup *pUp : static_cast<Character*>(images[ppl+"king"])->inventory){  
    pUp->setFixed(true);
    pUp->setPosition(x * 40, 66);
    if(!pUp->isActivated()){
      SDL_SetTextureAlphaMod(pUp->getTexture(), 255);
      x++;
    }
  }
  // shows contents of chest when open
  if(static_cast<Sprite*>(images[add+"key"])->pair->pair
    == static_cast<Sprite*>(images[add+"key"])->pair
    && !static_cast<Pickup*>(images[add+"key"])->isPickedUp())
  {
    SDL_SetTextureAlphaMod(images[add+"key"]->getTexture(), 255);
    static_cast<Character*>(images[ppl+"king"])
      ->inventory.push_back(static_cast<Pickup*>(images[add+"key"]));
    static_cast<Sprite*>(images[add+"key"])->pair
      = static_cast<Character*>(images[ppl+"king"]);
    static_cast<Pickup*>(images[add+"key"])->pickUp();
  }
  if(static_cast<Sprite*>(images[add+"coin"])->pair->pair
    == static_cast<Sprite*>(images[add+"coin"])->pair
    && !static_cast<Pickup*>(images[add+"coin"])->isPickedUp())
  {
    SDL_SetTextureAlphaMod(images[add+"coin"]->getTexture(), 255);
    static_cast<Character*>(images[ppl+"king"])
      ->inventory.push_back(static_cast<Pickup*>(images[add+"coin"]));
    static_cast<Sprite*>(images[add+"coin"])->pair
      = static_cast<Character*>(images[ppl+"king"]);
    engine->score += 1000;
    static_cast<Pickup*>(images[add+"coin"])->pickUp();
  }

  // updates Health
  updateHearts();
  State::update(seconds);

  // changes state to Lose
  if (static_cast<Character*>(images[ppl+"king"])->hearts <= 0) {
    engine->setState("lose");
  }
  // changes state to Win
  if (images[ppl+"king"]->pos_x < map->width/2 + 45
      && images[ppl+"king"]->pos_x + images[ppl+"king"]->getDestRect()->w
        > map->width/2 - 45
      && images[ppl+"king"]->pos_y + images[ppl+"king"]->getDestRect()->h
        > map->height - 150)
  {
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file << std::to_string(engine->score) << std::endl;
    file.close();
    engine->setState("win");
  }
  // go to floor 2 w/ '1'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    engine->setState("level_2");
   }, SDLK_1);
  // automatically win w/ '2'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
   engine->setState("win"); }, SDLK_2);
  // automatically lose w/ '3'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
   engine->setState("lose"); }, SDLK_3);
  // pause w/ 'p'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
   pause(); }, SDLK_p);
  //Delete instruction text by pressing 'n'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
   deactivateInstructionText(); }, SDLK_n);
}

void Level_3_State::checkFollow() {
  int borderX = images[ppl+"king"]->pos_x
    + images[ppl+"king"]->getDestRect()->w+100;
  int borderY = images[ppl+"king"]->pos_y
    + images[ppl+"king"]->getDestRect()->h+100;
  for (int i = 0; i < num_enemies; i++) {
    std::string s = ppl+"enemy_"+std::to_string(i);
    if (!static_cast<Character*>(images[s])->running
        && images[s]->pos_x
        >= images[ppl+"king"]->pos_x - 100 - images[s]->getDestRect()->w
        && images[s]->pos_y
        >= images[ppl+"king"]->pos_y - 100 - images[s]->getDestRect()->h
        && images[s]->pos_x <= borderX && images[s]->pos_y <= borderY)
    {
      static_cast<Enemy*>(images[s])->following = true;
    }
  }
}
      
void Level_3_State::enemyFollow() {
  for (int i = 0; i < num_enemies; i++) {
    std::string s = ppl+"enemy_"+std::to_string(i);
    if (static_cast<Enemy*>(images[s])->frozen){
      images[s]->velocityX = 0;
      images[s]->velocityY = 0;
      continue;
    }
    if (static_cast<Enemy*>(images[s])->following) {
      // edit x velocity
      if (images[s]->pos_x+32 < images[ppl+"king"]->pos_x) {
        images[s]->velocityX = 100;
      } else if (images[s]->pos_x > images[ppl+"king"]->pos_x+32) {
        images[s]->velocityX = -100;
      } else {
        images[s]->velocityX = 0;
      }
      // edit y velocity
      if (images[s]->pos_y < images[ppl+"king"]->pos_y-25) {
        images[s]->velocityY = 100;
      } else if (images[s]->pos_y > images[ppl+"king"]->pos_y+25) {
        images[s]->velocityY = -100;
      } else {
        images[s]->velocityY = 0;
      }
    }
  }
}

// inc/dec sta
void Level_3_State::updateSta() {
  int w = int(static_cast<Character*>(images[ppl+"king"])->sta * 146);  
  images[top+"sta_bar"]->getDestRect()->w = w;
}

// inc/dec exp
void Level_3_State::updateExp() {
  int w = int(static_cast<Character*>(images[ppl+"king"])->exp * 146);
  images[top+"exp_bar"]->getDestRect()->w = w;
}

//Health update with extra heart
void Level_3_State::updateHeartsPlus(){
  SDL_SetTextureAlphaMod(images[top+"heart_4"]->getTexture(), 255);
  switch(static_cast<Character*>(images[ppl+"king"])->hearts) {
    case 8:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 7:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 6:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 5:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 4:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 3:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 2:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 1:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(40, 0, 32, 32);
      break;
  }
}
//Health update with normal heart
void Level_3_State::updateHearts(){
  SDL_SetTextureAlphaMod(images[top+"heart_4"]->getTexture(), 0);
  for(unsigned n =0; n < Character::activePowerups.size(); n++){
    if(Character::activePowerups[n] == foodNum){
      updateHeartsPlus();
      return;
    }
  }

  switch(static_cast<Character*>(images[ppl+"king"])->hearts) {
    case 6:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 5:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 4:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 3:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 2:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 1:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(40, 0, 32, 32);
      break;
  }
}
Level_3_State::~Level_3_State() {}
