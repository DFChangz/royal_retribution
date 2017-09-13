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

  images.push_back(new Image(renderer, surf, "assets/space.jpg", &error_handler));
  images.push_back(new Character(renderer, surf, "assets/blue.png",
    &error_handler, 5, 5, &eventHandler));

  eventHandler.addListener(SDL_QUIT, [&] () {running = false;});
  eventHandler.addListener(SDL_KEYUP, [&] () {running = false;}, SDLK_ESCAPE);
}

// Load the assets and create textures
void Engine::load() {
  for (Image* image : images) {
    image->load();
  }
}

// The heart
void Engine::loop() {
  unsigned int lastTime = SDL_GetTicks();
  double totalTime = 0;

  while(running) {
    unsigned int currentTime = SDL_GetTicks();
    double seconds = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;
    totalTime += seconds;

    eventHandler.check(); 

    update(seconds);

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
  for (Image* image : images) {
    if (image != nullptr) {
      delete image;
    }
  }

  if (surf != nullptr) {
    SDL_FreeSurface(surf);
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
  error_handler(this) {};
