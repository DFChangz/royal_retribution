#ifndef ENGINE_H
#define ENGINE_H

#define WIDTH   1280 
#define HEIGHT  720
#define WINDOW_TITLE "NAME OF GAME"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "error_handler.h"
#include "image.h"

class Engine {
  public:
    void start();
    ErrorHandler error_handler;
    void cleanup();

    Image *bg_image;

    Engine();
  private:
    void setup();
    void load();
    void loop();
    void render();
		SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surf = nullptr;
};

#endif
