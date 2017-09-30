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

/*Header file for the game engine that is in charge of setup loading, 
and rendering textures along with any audio and text. Plus it destroys 
objects and frees memory when done*/
class State;
class Engine {
  public:
    void start();
    void cleanup();
    void newGame();
    void newHighscore();

    Engine();

    ErrorHandler error_handler;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    void setState(std::string);

    void quit();
    void incrementScore(int increment);
    int score = 0;
    std::map<std::string, State*> states;
  private:
    void setup();
    void loop();
    void update(double seconds);
    void render();
    void createStates();
    bool running = true;
    EventHandler eventHandler;
    State* currentState = nullptr;
    Mix_Music *intro = nullptr;
};

#endif
