/*
 * audio.cpp
 */

#include "audio.h"
//Definition of the class that handles audio

//constructor takes in error handler
Audio::Audio(ErrorHandler* error_handler) : error_handler(error_handler) {}

//Loads in the collision sound only sound we have right now
void Audio::load() {
  Mix_Music* intro = Mix_LoadMUS(INTRO_FILENAME);
  if (intro == nullptr) {
    error_handler->quit(__func__, Mix_GetError());
  }
  Mix_Music* sound = Mix_LoadMUS(COLLISION_FILENAME);
  if (sound == nullptr) {
    error_handler->quit(__func__, Mix_GetError());
  }
  sounds["intro"] = intro;
  sounds["collision"] = sound;
}

void Audio::cleanup() {
  for (auto sound : sounds) {
    Mix_FreeMusic(sound.second);
  }
}
//plays the collision sound
void Audio::play(std::string sound, int loops) {
  if (Mix_PlayMusic(sounds[sound], loops) < 0) {
    error_handler->quit(__func__, Mix_GetError());
  }
}
