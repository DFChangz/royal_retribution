#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <vector>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "error_handler.h"
#include "image.h"
#include "sprite.h"
#include "character.h"
#include "enemy.h"
#include "event_handler.h"
#include "constants.h"
#include "collision_detector.h"
#include "audio.h"
#include "state.h"

class State;
class Engine {
  public:
    void start();
    void cleanup();

    Engine();

    ErrorHandler error_handler;
    SDL_Renderer *renderer;

    void setState(std::string);
  private:
    void setup();
    void loop();
    void update(double seconds);
    void render();
    void createStates();
    bool running = true;
		SDL_Window *window;
    EventHandler eventHandler;
    std::map<std::string, State*> states;
    State* currentState;
};

#endif
