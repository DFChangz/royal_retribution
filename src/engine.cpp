/*
 * engine.cpp
 */

#include "engine.h"
#include "win_state.h"
#include "lose_state.h"
#include "menu_state.h"
#include "title_state.h"
#include "intro_state.h"
#include "credit_state.h"
#include "playing_state.h"
#include "level_2_state.h"
#include "level_3_state.h"
#include "highscore_state.h"
#include "instruction_state.h"

// Starts the game
int Engine::volume = 100;
void Engine::start() {
  setup();
  loop();
  cleanup();
}

// Runs any setup code that is needed to load the assets.
void Engine::setup() {
  srand (time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){ 
    error_handler.quit(__func__, SDL_GetError());
  }
  // DOES NOT FUNCTION CORRECTLY IN MAC OS X.
  if ((Mix_Init(MIX_INIT_MOD) & MIX_INIT_MOD) == 0) {
    //error_handler.quit(__func__, Mix_GetError());
  }
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
    error_handler.quit(__func__, IMG_GetError());
  }
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
    error_handler.quit(__func__, Mix_GetError());
  }
  if (TTF_Init() < 0) {
    error_handler.quit(__func__, TTF_GetError());
  }
  //window setup
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  int disp_width = DM.w;
  int disp_height = DM.h;
  int pos_x = disp_width / 2 - WIDTH / 2;
  int pos_y = disp_height / 2 - HEIGHT / 2;

  window = SDL_CreateWindow(WINDOW_TITLE, pos_x, pos_y, WIDTH, HEIGHT, 
    SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    error_handler.quit(__func__, SDL_GetError());
  }
  
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | 
    SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    error_handler.quit(__func__, SDL_GetError());
  }


  createStates();
  setState("title");

  //Quits on escape.
  eventHandler.addListener(SDL_QUIT, [&] (SDL_Event*) {quit();});
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {quit();}, SDLK_ESCAPE);
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    setState("menu");
    Mix_HaltMusic();
    }, SDLK_q);
}

// The heart
void Engine::loop() {
  lastTime = SDL_GetTicks();
  double totalTime = 0;

  while(running) {
    //converts time to seconds and keeps track of time passed and total time
    unsigned int currentTime = SDL_GetTicks();
    double seconds = 0;
    seconds += (currentTime - lastTime) / 1000.0;
    accumulator += seconds;
    totalTime += seconds;
    lastTime = currentTime;

    while (accumulator >= DELTA_TIME) {
      eventHandler.getEvents();
      eventHandler.check();

      currentState->run(&accumulator);
    }

    double interpol_alpha = accumulator/DELTA_TIME;
    currentState->render(interpol_alpha);

    SDL_Delay((DELTA_TIME - accumulator)*1000);
  }
}

// Cleanup all resources before quitting
void Engine::cleanup() {
  for (auto state : states) {
    if (state.second != nullptr) {
      delete state.second;
    }
  }

  if (window != nullptr) {
    SDL_DestroyWindow(window);
  }
  if (renderer != nullptr) {
    SDL_DestroyRenderer(renderer);
  }

  Mix_FreeMusic(intro);

  IMG_Quit();
  TTF_Quit();
  Mix_Quit();
  SDL_Quit();
}
 
void Engine::incrementScore(int increment){
  this->score += increment;
}

void Engine::setState(std::string state) {
  if (states.find(state) == states.end()) {
    error_handler.quit(__func__, "A state with this name does not exist");
  }
  currentState = states[state];
}

void Engine::createStates() {
  states["win"] = new WinState(this, &error_handler);
  states["lose"] = new LoseState(this, &error_handler);
  states["menu"] = new MenuState(this, &error_handler);
  states["title"] = new TitleState(this, &error_handler);
  states["credits"] = new CreditState(this, &error_handler);
  states["intro"] = nullptr;
  states["playing"] = nullptr;
  states["level_2"] = nullptr;
  states["level_3"] = nullptr;
  states["instruction"] = nullptr;
  newGame();
  states["Highscore"] = nullptr;
  newHighscore();
}


Engine::Engine() : 
  error_handler(this) {};

void Engine::quit() {
  running = false;
}

void Engine::newGame() {
  if (states["playing"] != nullptr) {
    delete states["playing"];
    states["playing"] = nullptr;
  }
  if (states["level_2"] != nullptr) {
    delete states["Level_2"];
    states["Level_2"] = nullptr;
  }
  if (states["level_3"] != nullptr) {
    delete states["level_3"];
    states["level_3"] = nullptr;
  }
  if (states["win"] != nullptr) {
    delete states["win"];
    states["win"] = nullptr;
  }
  if (states["lose"] != nullptr) {
    delete states["lose"];
    states["lose"] = nullptr;
  }
  if (states["intro"] != nullptr) {
    delete states["intro"];
    states["intro"] = nullptr;
  }
  if (states["instruction"] != nullptr) {
    delete states["instruction"];
    states["instruction"] = nullptr;
  }
  score = 0;
  states["win"] = new WinState(this, &error_handler);
  states["lose"] = new LoseState(this, &error_handler);
  states["intro"] = new IntroState(this, &error_handler);
  states["playing"] = new PlayingState(this, &error_handler);
  states["level_2"] = new Level_2_State(this, &error_handler);
  states["level_3"] = new Level_3_State(this, &error_handler);
  states["instruction"] = new InstructionState(this, &error_handler);

  while(!Character::inventory.empty()){
    Character::inventory.pop_back();
  }
  while(!Character::activePowerups.empty()){
    Character::activePowerups.pop_back();
  }

  Character::hearts = 6;
  Character::level = 1;
  Character::exp = 0;

  lastTime = SDL_GetTicks();
}

/*void Engine::setNextLevel(std::string level, Image* &sentKing) {
  if (states[level] != nullptr) {
    delete states[level];
    states[level] = nullptr;
  }
  states[level] = new Level_2_State(this, &error_handler, sentKing);
  setState(level);
}*/

void Engine::newHighscore() {
  if (states["Highscore"] != nullptr) {
    delete states["Highscore"];
    states["Highscore"] = nullptr;
  }
  states["Highscore"] = new HighscoreState(this, &error_handler);
}
