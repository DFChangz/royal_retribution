#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "error_handler.h"
#include "image.h"
#include "sprite.h"
#include "character.h"
#include "event_handler.h"
#include "constants.h"

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
    void update(double seconds);
    void render();
		SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surf = nullptr;
    EventHandler eventHandler;
    bool running = true;
};

#endif
