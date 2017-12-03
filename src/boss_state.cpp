/*
 * playing_state.cpp
 */

#include "boss_state.h"
#include "big_alien.h"
#include "main_boss.h"

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
  images[add+"cLight"] = new Sprite(engine->renderer, CAMERA_LIGHT,
    errorHandler, 0, 0, false, true);
  // Black
  images[add+"black"] = new Sprite(engine->renderer, BLACK_PIXEL, errorHandler,
    0, 0, false);
  // Skip Pan Test
  images[top+"skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH/2-170, HEIGHT-35, 30, "Press [m] to Skip Pan", ROYAL_GOLD);

  // Player 
  if (king != nullptr)
    images[ppl+"king"] = king;
  else
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, 0, 0, &eventHandler, &audioHandler, this);
  images[ppl+"king"]->setPosition(map->width/2 - 16, map->height/2 + 10);
  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, map->width/2, map->height/2, static_cast<Sprite*>
    (images[ppl+"king"]), &eventHandler, &audioHandler, this);

  // Big Alien
  images[ppl+"eBigAlien"] = new BigAlien(engine->renderer, BIG_BODY,
    errorHandler, 200, 160, map->width/2 - 100, map->height/2 - 385, 8,
    static_cast<Sprite*>(images[ppl+"king"]));
  // head
  images[ppl+"zbigHead"] = new Sprite(engine->renderer, BIG_HEAD,
    errorHandler, 200, 200, map->width/2 - 100, map->height/2 - 505, false);
  // left hand
  images[ppl+"bigLF"] = new Hand(engine->renderer, BIG_LF,
    errorHandler, 160, 160, map->width/2 + 100, map->height/2 - 385, 0, 0, 5);
  // right hand
  images[ppl+"bigRF"] = new Hand(engine->renderer, BIG_RF,
    errorHandler, 160, 160, map->width/2 - 260, map->height/2 - 385, 0, 0, 5);
  // set body parts
  static_cast<BigAlien*>(images[ppl+"eBigAlien"])
    ->setHands(static_cast<Hand*>(images[ppl+"bigRF"]),
      static_cast<Hand*>(images[ppl+"bigLF"]));
  static_cast<BigAlien*>(images[ppl+"eBigAlien"])
    ->setHead(static_cast<Sprite*>(images[ppl+"zbigHead"]));

  // Main Boss
  images[ppl+"eMainBoss"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, map->width/2 - 16, map->height/2 + 70, 0, 0, 5, map);
  images[ppl+"eclone1"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, map->width/2 - 66, map->height/2 + 70, 0, 0, 5, map);
  images[ppl+"eclone2"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, map->width/2 + 46, map->height/2 + 70, 0, 0, 5, map);
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])
    ->clone1 = static_cast<MainBoss*>(images[ppl+"eclone1"]);
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])
    ->clone2 = static_cast<MainBoss*>(images[ppl+"eclone2"]);
  static_cast<MainBoss*>(images[ppl+"eclone1"])
    ->setClone(true);
  static_cast<MainBoss*>(images[ppl+"eclone2"])
    ->setClone(true);
  static_cast<Enemy*>(images[ppl+"eMainBoss"])
    ->followWhenClose(images[ppl + "king"], FOLLOW_RADIUS);

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
  //fps
  images[top+"fps"] = new Text(engine->renderer, FONT_FILENAME,  errorHandler,
    2, 2, 16, "FPS: ");

  // automatically win w/ '2'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file.close();
    engine->setState("win"); 
  }, SDLK_2);

  // go to floor 4 w/ '1'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    Character::currFloor = 4;
    engine->setState("level_4");
   }, SDLK_1);
}

void BossState::load() {
  State::load();
  // sta and exp
  images[top+"sta_box"]->getDestRect()->h = 32;
  images[top+"sta_box"]->getDestRect()->w = 150;
  images[top+"sta_bar"]->getDestRect()->h = 24;
  images[top+"sta_bar"]->getDestRect()->w = 146;
  images[top+"exp_box"]->getDestRect()->h = 32;
  images[top+"exp_box"]->getDestRect()->w = 150;
  images[top+"exp_bar"]->getDestRect()->h = 24;
  images[top+"exp_bar"]->getDestRect()->w = 148;
  // bosses
  SDL_SetTextureAlphaMod(images[ppl+"eclone1"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[ppl+"eclone2"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[ppl+"eMainBoss"]->getTexture(), 0);
  camera.setPosition(images[ppl+"zbigHead"]);
  // shading
  images[add+"black"]->getDestRect()->w = map->width;
  images[add+"black"]->getDestRect()->h = map->height;
  SDL_SetTextureBlendMode(images[add+"black"]
    ->getTexture(), SDL_BLENDMODE_ADD);
  if (SDL_SetTextureAlphaMod(images[add+"black"]->getTexture(), 150) < 0) {
    errorHandler->quit(__func__, SDL_GetError());
  }
  images[add+"cLight"]->getDestRect()->w = WIDTH;
  images[add+"cLight"]->getDestRect()->h = HEIGHT;
  SDL_SetTextureBlendMode(images[add+"cLight"]
    ->getTexture(),SDL_BLENDMODE_MOD);
  
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (skipPan) pause(); }, SDLK_p);
  //Delete instruction text / resume by pressing 'r'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (isPaused()) resume();
  }, SDLK_r);
}

void BossState::update(double seconds) {
  // EPIC boss music
  if(musicSwitch == 1){
    audioHandler.play("boss");
    musicSwitch = 2;
  }
  PlayingState::update(seconds);
  // fade out the head with the body
  if (static_cast<BigAlien*>(images[ppl+"eBigAlien"])->isDying())
    fade = fadeOut(ppl+"zbigHead", fade, seconds, 1.0);
  // if Big Alien dies
  if(static_cast<Enemy*>(images[ppl+"eBigAlien"])->isDead() && thePhase != 1){
    thePhase = static_cast<MainBoss*>(images[ppl+"eMainBoss"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone1"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone2"])->changePhase(); 
    SDL_SetTextureAlphaMod(images[ppl+"eMainBoss"]->getTexture(), 255);
    SDL_SetTextureAlphaMod(images[ppl+"eclone1"]->getTexture(), 255);
    SDL_SetTextureAlphaMod(images[ppl+"eclone2"]->getTexture(), 255);
  }
  if(static_cast<Boss_Enemy*>(images[ppl+"eMainBoss"])->getHp() == 1){
    thePhase++;
  }
  if(thePhase == 2){
    static_cast<MainBoss*>(images[ppl+"eMainBoss"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone1"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone2"])->changePhase();
  }   
  if(static_cast<Enemy*>(images[ppl+"eMainBoss"])->isDead()){
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file.close();
    engine->setState("win"); 
    
  }
}

BossState::~BossState() {}
