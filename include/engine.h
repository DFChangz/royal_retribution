#ifndef ENGINE_H
#define ENGINE_H

#define WIDTH   1280 
#define HEIGHT  720
#define WINDOW_TITLE "NAME OF GAME"

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "error_handler.h"
#include "image.h"
#include "event_handler.h"

class Engine {
  public:
    void start();
    ErrorHandler error_handler;
    void cleanup();

    std::vector<Image*> images;

    Engine();
  private:
    void setup();
    void load();
    void loop();
    void update();
    void render();
		SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surf = nullptr;
    EventHandler eventHandler;
    bool running = true;
};

#endif
