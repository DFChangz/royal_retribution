#include "engine.h"

// Starts the game
void Engine::start() {
  setup();

  load();

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

  //renders bg image and player character
  images.push_back(new Image(renderer, BG_FILENAME, &error_handler));
  images.push_back(new Character(renderer, CHARACTER_FILENAME,
    &error_handler, 0, 0, &eventHandler, &audio_handler));

  //spawn 5 enemiies at random locations
  for (int i = 0; i < 5; i++) {
    images.push_back(new Enemy(renderer, ENEMY_FILENAME,
      &error_handler, rand() % WIDTH, rand() % HEIGHT, rand() % 100 + 1,
      rand() % 100 + 1));
  }

  //Quits on escape.
  eventHandler.addListener(SDL_QUIT, [&] () {running = false;});
  eventHandler.addListener(SDL_KEYUP, [&] () {running = false;}, SDLK_ESCAPE);
}

// Load the assets and create textures
void Engine::load() {
  audio_handler.load();

  for (Image* image : images) {
    image->load();
  }
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

    //checks the handlers updates objects and renders them on the screen
    eventHandler.check(); 

    update(seconds);

    collision_detector.check();

    render();
  }
}

void Engine::update(double seconds) {
  for (Image* image : images) {
    image->update(seconds);
  }
}

// Render each asset
void Engine::render() {
  if (SDL_RenderClear(renderer) < 0) {
    error_handler.quit(__func__, SDL_GetError());
  }

  for (Image* image : images) {
    image->render();
  }

  SDL_RenderPresent(renderer);
}

// Cleanup all resources before quitting
void Engine::cleanup() {
  audio_handler.cleanup();

  for (Image* image : images) {
    if (image != nullptr) {
      delete image;
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

Engine::Engine() : 
  collision_detector(&images), error_handler(this), audio_handler(&error_handler) {};
