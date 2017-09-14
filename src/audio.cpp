#include "audio.h"

Audio::Audio(ErrorHandler* error_handler) : error_handler(error_handler) {}

void Audio::load() {
  Mix_Music* sound = Mix_LoadMUS(COLLISION_FILENAME);
  if (sound == nullptr) {
    error_handler->quit(__func__, Mix_GetError());
  }
  sounds["collision"] = sound;
}

void Audio::cleanup() {
  for (auto sound : sounds) {
    Mix_FreeMusic(sound.second);
  }
}

void Audio::play(std::string sound, int loops) {
  if (Mix_PlayMusic(sounds[sound], loops) < 0) {
    error_handler->quit(__func__, Mix_GetError());
  }
}
