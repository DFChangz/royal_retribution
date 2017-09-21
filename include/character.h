#ifndef CHARACTER_H
#define CHARACTER_H

#define CHARACTER_FPS   5
#define RUNNING_FRAMES  4
#define IDLE_FRAMES     1
#define R_RUNNING_POS   5
#define L_RUNNING_POS   0
#define DOWN_IDLE_POS   10
#define UP_IDLE_POS     11

#include "sprite.h"
#include "event_handler.h"
#include "constants.h"
#include "audio.h"

/*class for the player character that inherits from sprite will be 
responsible for player movement events and holds certain sfx along with
its position and rect*/
class Character : public Sprite {
  public:
    Character(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, EventHandler*, Audio*);

    Character(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
     int pos_x, int pos_y, EventHandler*, Audio*);

    virtual void notifyCollision(Image*, SDL_Rect* intersection);

    virtual void load();
    virtual void update(double seconds);
  private:
    void createListeners(EventHandler*);
    Audio *audioHandler = nullptr;
    void idleAnimation(double seconds);

    std::string dir = "right";
};

#endif
