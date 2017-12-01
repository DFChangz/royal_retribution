/*
 * playing_state.cpp
 */

#include "boss_state.h"
#include "big_alien.h"

BossState::BossState(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, FINAL_LEVEL, TILES_TXT,
    &collisionDetector);
//  map->loadSecondTextures(TILES_ADD);
//  map->loadSecondLayout(LEVEL_1_ADD);

  setup();
  load();
}

void BossState::setup() {
  //Camera Light
  images[add+"cLight"] = new Sprite(engine->renderer, CAMERA_LIGHT, errorHandler,
    0,0, false, true);
  // Hole
  images[ele+"hole"] = new Sprite(engine->renderer, BLACK_PIXEL,
    errorHandler, 0, 0, false);
  // Player 
  if (king != nullptr) {
    images[ppl+"king"] = king;
  } else {
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, 1094, 1074, &eventHandler, &audioHandler, this);
  }
  //Big Alien  
  images[ppl+"bigAlien"] = new BigAlien(engine->renderer, BIG_BODY,
    errorHandler, 160, 160, 1032, 800, 0, 0, 20);
  images[ppl+"bigHead"] = new Sprite(engine->renderer, BIG_HEAD, errorHandler,160, 160,
    1032, 670, false);
  images[ppl+"bigLF"] = new Hand(engine->renderer, BIG_LF,
    errorHandler, 160, 160, 1240, 960, 0, 0);
  images[ppl+"bigRF"] = new Hand(engine->renderer, BIG_RF,
    errorHandler, 160, 160, 825, 960, 0, 0);
  static_cast<BigAlien*>(images[ppl+"bigAlien"])->setHands(static_cast<Hand*>(images[ppl+"bigRF"]),static_cast<Hand*>(images[ppl+"bigLF"]));
  static_cast<BigAlien*>(images[ppl+"bigAlien"])->setHead(static_cast<Sprite*>(images[ppl+"bigHead"]));

  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, 1094, 1074, static_cast<Sprite*>(images[ppl+"king"]), &eventHandler,
    &audioHandler, this);
  // Enemies
  /*std::ifstream file(LEVEL_1_E);
  int x = -1;
  int y = -1;

  while ((file >> y) && y != -1 && (file >> x) && x != -1) {
    std::string s = ppl+"enemy_"+std::to_string(num_enemies);
    images[s] = new Enemy(engine->renderer, ANI_FILENAME, errorHandler,
      16, 25, (x-1) * TILE_DIM, (y-1) * TILE_DIM, 0, 150);
    static_cast<Enemy*>(images[s])->followWhenClose(images[ppl + "king"],
      FOLLOW_RADIUS);
    num_enemies++;
  }
  file.close();*/

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
  images[top+"level"] = new Text(engine->renderer, FONT_FILENAME,
    errorHandler, WIDTH/2-48, 2, 16, "KING LEVEL "); 
  // Heart types
  images[top+"heart_1"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 40, 34, false, true);
  images[top+"heart_2"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 80, 34, false, true);
  images[top+"heart_3"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 120, 34, false, true);
  images[top+"heart_4"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 160, 34, false, true);
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

  // instuctions
  setupInstruct();
  //fps
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
}
void BossState::load() {
  State::load();
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

    images[s]->getDestRect()->w = 3*TILE_DIM;
    images[s]->getDestRect()->h = 3*TILE_DIM;

    SDL_SetTextureBlendMode(images[s]->getTexture(),SDL_BLENDMODE_ADD);
  }

  images[add+"black"]->getDestRect()->w = map->width;
  images[add+"black"]->getDestRect()->h = map->height;

  SDL_SetTextureBlendMode(images[add+"black"]
    ->getTexture(), SDL_BLENDMODE_ADD);
  images[add+"cLight"]->getDestRect()->w = WIDTH;
  images[add+"cLight"]->getDestRect()->h = HEIGHT;

  SDL_SetTextureBlendMode(images[add+"cLight"]
    ->getTexture(),SDL_BLENDMODE_MOD);

  // center some of the textures
  for (it = images.begin(); it != images.end(); it++) {
    if (it->first == "3skip") {
      auto center = getCenterForImage(it->second);
      it->second->setPosition(std::get<0>(center), HEIGHT-35);
    } else if (it->first == "3level") {
      auto center = getCenterForImage(it->second); 
      it->second->setPosition(std::get<0>(center), 0);
    } else if (it->first == "3dkInstruct"
               || it->first == "3cInstruct"
               || it->first == "3hInstruct"
               || it->first == "3tInstruct"
               || it->first == "3fInstruct"
               || it->first == "3FInstruct")
    {
      auto center = getCenterForImage(it->second);
      it->second->setPosition(std::get<0>(center), std::get<1>(center)-80);
    }
  }
  deactivateInstructionText();
  camera.setPosition(images[ppl+"bigAlien"]);

}

void BossState::update(double seconds) {
  
  
  if(musicSwitch == 1){
    audioHandler.play("boss");
    musicSwitch = 2;
  }
  PlayingState::update(seconds);


}

BossState::~BossState() {}
