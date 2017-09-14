#ifndef AUDIO_H 
#define AUDIO_H

#include <iostream>
#include <map>
#include <SDL.h>
#include <SDL_mixer.h>
#include "constants.h"
#include "error_handler.h"

/*Header file for the class that handles audio, Responsible for loading 
and holding music as well as playing it*/
class Audio {
  public:
    Audio(ErrorHandler*);

    void load();
    void cleanup();
    void play(std::string, int = -1);
  private:
    ErrorHandler* error_handler;
    std::map<std::string, Mix_Music*> sounds;
};

#endif
