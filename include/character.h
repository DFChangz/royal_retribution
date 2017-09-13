#ifndef CHARACTER_H
#define CHARACTER_H

#include "sprite.h"
#include "event_handler.h"
#include "constants.h"

class Character : public Sprite {
  public:
    Character(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
      EventHandler*);

    Character(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler, int pos_x, int pos_y, EventHandler*);

  private:
    void createListeners(EventHandler*);
};

#endif
