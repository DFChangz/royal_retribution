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
#include "enemy.h"
#include "event_handler.h"
#include "constants.h"
#include "collision_detector.h"
#include "audio.h"

/*Header file for the game engine that is in charge of setup loading, 
and rendering textures along with any audio and text. Plus it destroys 
objects and frees memory when done*/
class Engine {
  public:
    void start();
    void cleanup();

    Engine();
  private:
    void setup();
    void load();
    void loop();
    void update(double seconds);
    void render();
		SDL_Window *window;
    SDL_Renderer *renderer;
    EventHandler eventHandler;
    bool running = true;
    CollisionDetector collision_detector;
    ErrorHandler error_handler;
    std::vector<Image*> images;

    Audio audio_handler;
};

#endif
