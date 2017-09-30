/*
 * audio.cpp
 */

#include "audio.h"
//Definition of the class that handles audio

//constructor takes in error handler
Audio::Audio(ErrorHandler* error_handler) : error_handler(error_handler) {}

//Loads in the collision sound only sound we have right now
void Audio::load() {
  // First channel for music, second for sound effects
  if (Mix_AllocateChannels(1) != 1) {
    error_handler->quit(__func__, "Failed to allocate channels!"); 
  }

  Mix_Music* intro = Mix_LoadMUS(INTRO_FILENAME);
  if (intro == nullptr) {
    error_handler->quit(__func__, Mix_GetError());
  }
  music["intro"] = intro;


  Mix_Chunk* sound = Mix_LoadWAV(COLLISION_FILENAME);
  if (sound == nullptr) {
    error_handler->quit(__func__, Mix_GetError());
  }
  sound_effects["collision"] = sound;

  sound = Mix_LoadWAV(KILL_FILENAME);
  if (sound == nullptr) {
    error_handler->quit(__func__, Mix_GetError());
  }
  sound_effects["kill"] = sound;
}

void Audio::cleanup() {
  for (auto mus : music) {
    Mix_FreeMusic(mus.second);
  }

  for (auto chunk : sound_effects) {
    Mix_FreeChunk(chunk.second);
  }
}
//plays the collision sound
void Audio::play(std::string sound, int loops) {
  if (music.find(sound) != music.end()) {
    if (Mix_PlayMusic(music[sound], loops) < 0) {
      error_handler->quit(__func__, Mix_GetError());
    }
  } else if (sound_effects.find(sound) != sound_effects.end()) {
    if (Mix_PlayChannel(0, sound_effects[sound], loops) < 0) {
      error_handler->quit(__func__, Mix_GetError());
    }
  } else {
    error_handler->quit(__func__, "Sound not found.");
  }
}
