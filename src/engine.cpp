#include "engine.h"
#include "playing_state.h"
#include "title_state.h"
#include "menu_state.h"

// Starts the game
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
  eventHandler.addListener(SDL_QUIT, [&] () {running = false;});
  eventHandler.addListener(SDL_KEYUP, [&] () {running = false;}, SDLK_ESCAPE);
}

// The heart
void Engine::loop() {
  unsigned int lastTime = SDL_GetTicks();
  double totalTime = 0;

  while(running) {
    //converts time to seconds and keeps track of time passed and total time
    unsigned int currentTime = SDL_GetTicks();
    double seconds = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;
    totalTime += seconds;

    eventHandler.getEvents();
    eventHandler.check();

    currentState->run(seconds);
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

  IMG_Quit();
  TTF_Quit();
  Mix_Quit();
  SDL_Quit();
} 

void Engine::setState(std::string state) {
  if (states.find(state) == states.end()) {
    error_handler.quit(__func__, "A state with this name does not exist");
  }

  currentState = states[state];
}

void Engine::createStates() {
  states["menu"] = new MenuState(this, &error_handler);
  states["title"] = new TitleState(this, &error_handler);
  states["playing"] = new PlayingState(this, &error_handler);
}

Engine::Engine() : 
  error_handler(this) {};
