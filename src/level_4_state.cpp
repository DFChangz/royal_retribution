/*
 * level_4_state.cpp
 */

#include "level_4_state.h"

Level_4_State::Level_4_State(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_4, TILES_TXT,
    &collisionDetector);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_4_ADD);
  setup();
  load();
}

void Level_4_State::setup() {
  // Stairs 
  images[ele+"stairs"] = new Sprite(engine->renderer, STAIRS_FILENAME,
    errorHandler, 670, 853, false);
  // Hole
  images[ele+"hole"] = new Sprite(engine->renderer, BLACK_PIXEL,
    errorHandler, 0, 0, false);
  // King
  if (king != nullptr) {
    images[ppl+"king"] = king;
  } else {
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, 126, 210, &eventHandler, &audioHandler, this);
  }
  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, 100, 300, static_cast<Sprite*>(images[ppl+"king"]), &eventHandler,
    &audioHandler, this);
  // Enemies
  std::ifstream file(LEVEL_4_E);
  int x = -1;
  int y = -1;
  int num_mini = 0;
  while ((file >> y) && y != -1 && (file >> x) && x != -1) {
    std::string s = "";
    if (num_mini >= 8) {
      s = ppl+"enemy_"+std::to_string(num_enemies);
      images[s] = new Enemy(engine->renderer, ANI_FILENAME, errorHandler,
        16, 25, (x-1) * TILE_DIM, (y-1) * TILE_DIM, 0, 150);
        num_enemies++;
    } else {
      s = ppl+"enemy_mini_"+std::to_string(num_mini);
      images[s] = new Mini_Enemy(engine->renderer, ANI_FILENAME, errorHandler,
        16, 25, (x-1) * TILE_DIM, (y-1) * TILE_DIM, 0, 0);
        num_mini++;
    }
    static_cast<Enemy*>(images[s])->followWhenClose(images[ppl + "king"],
      FOLLOW_RADIUS);
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
  // set coin pos
  int currMini = 0;
  double coinPos1X = 0.0;
  double coinPos1Y = 0.0;
  double coinPos2X = 0.0;
  double coinPos2Y = 0.0;
  Sprite *C1 = nullptr;
  Sprite *C2 = nullptr;
  for(auto tile : map->additions){
    if(tile.image->isChest()){
      if(coinPos1X == 0.0 && coinPos1Y == 0.0){
        coinPos1X = tile.image->pos_x;
        coinPos1Y = tile.image->pos_y;
        C1 = tile.image;
      } else if (coinPos2X == 0.0 && coinPos2Y == 0.0){
        coinPos2X = tile.image->pos_x;
        coinPos2Y = tile.image->pos_y;
        C2 = tile.image;
      } else {
        if(currMini < num_mini){
          static_cast<Enemy*>(images[ppl+"enemy_mini_"+std::to_string(currMini)])->
            setPosition(tile.image->pos_x, tile.image->pos_y);   
          static_cast<Enemy*>(images[ppl+"enemy_mini_"+std::to_string(currMini)])->
            setPair(tile.image);
            currMini++;
        }        
      }
    }
  }
// moving blades
  for(auto tile : map->additions){
    if(tile.image->isBlade()){
      tile.image->setVelocity(50, 0);
      tile.image->setCollidable(true);
    }
  }
  // add coin
  images[add+"coin1"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPos1X, coinPos1Y, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin1"])->setPair(C1);
  // add coin
  images[add+"coin2"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPos2X, coinPos2Y, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin2"])->setPair(C2);
  //instuctions
  images[top+"dkInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU FOUND A KEY! It opens a special door with 'e'. Press 'r' to clear text");
  images[top+"fInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "You got food, you now have an extra heart. Press 'r' to clear text");
  images[top+"hInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU FELL DOWN A HOLE! You are now going to the previous floor. Press 'r' to clear text");
  images[top+"tInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU ARE TRAPPED! Kill an enemy to escape! press 'r' to clear text ");
  images[top+"cInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU OPENED A CHEST! The item is now in your inventory. press 'r' to clear text");
  // FPS Counter 
  images[add+"fps"] = new Text(engine->renderer, FONT_FILENAME,  errorHandler,
    2, 2, 16, "FPS: ");

  // automatically win w/ '2'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file << std::to_string(engine->score) << std::endl;
    file.close();
    engine->setState("win"); 
  }, SDLK_2);

  // go to floor 3 w/ '1'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    Character::currFloor = 3;
    engine->setState("level_3");
   }, SDLK_1);
}
void Level_4_State::load() {
  PlayingState::load(); 
  //sets pickup texture to see thru when in chests
  SDL_SetTextureAlphaMod(images[add+"coin1"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"coin2"]->getTexture(), 0);
}

void Level_4_State::update(double seconds) {
  PlayingState::update(seconds);

  auto character = static_cast<Character*>(images[ppl+"king"]);
  auto coin1 = static_cast<Pickup*>(images[add+"coin1"]);
  if(static_cast<Sprite*>(images[add+"coin1"])->pair->pair
    == static_cast<Sprite*>(images[add+"coin1"])->pair
    && !coin1->isPickedUp())
  {

    character->pickUp(coin1);
    //subscribe for lines below?

    static_cast<Sprite*>(images[add+"coin1"])->pair = character;
    engine->score += 1000;
    activateInstructionText(chestNum);
  }
  auto coin2 = static_cast<Pickup*>(images[add+"coin2"]);
  if(static_cast<Sprite*>(images[add+"coin2"])->pair->pair
    == static_cast<Sprite*>(images[add+"coin2"])->pair
    && !coin2->isPickedUp())
  {
    character->pickUp(coin2);

    static_cast<Sprite*>(images[add+"coin2"])->pair = character;
    engine->score += 1000;
    activateInstructionText(chestNum);
  }

  // changes state to Win
  if (images[ppl+"king"]->pos_x < 720
      && images[ppl+"king"]->pos_y < 903
      && images[ppl+"king"]->pos_x+32 > 670
      && images[ppl+"king"]->pos_y+50 > 853)
  {
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file << std::to_string(engine->score) << std::endl;
    file.close();
    engine->setState("win");
  }
}

Level_4_State::~Level_4_State() {}