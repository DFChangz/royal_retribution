#ifndef CHARACTER_H
#define CHARACTER_H

#include "sprite.h"
#include "event_handler.h"
#include "constants.h"
#include "audio.h"

class Character : public Sprite {
  public:
    Character(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
      EventHandler*, Audio*);

    Character(SDL_Renderer *renderer, SDL_Surface *surf, std::string,
      ErrorHandler *error_handler, int pos_x, int pos_y, EventHandler*, Audio*);

    virtual void notifyCollision(Image*, SDL_Rect* intersection);

    virtual void load();
    virtual void update(double seconds);
  private:
    void createListeners(EventHandler*);

    Audio *audioHandler;
};

#endif
